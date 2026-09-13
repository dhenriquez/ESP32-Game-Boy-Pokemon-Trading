#include "bills_pc_storage.h"
#include <LittleFS.h>
#include "gb_trade_engine.h"

BillsPCStorageManager BillsPC;

BillsPCStorageManager::BillsPCStorageManager() {}

bool BillsPCStorageManager::begin() {
    if (!LittleFS.begin(true)) {
        Serial.println("[BillsPC] LittleFS Mount Failed");
        return false;
    }
    Serial.println("[BillsPC] LittleFS Mounted Successfully");

    // Check if bills_pc.json exists, if not initialize with empty array
    if (!LittleFS.exists(BILLS_PC_FILE)) {
        File file = LittleFS.open(BILLS_PC_FILE, "w");
        if (file) {
            file.print("[]");
            file.close();
            Serial.println("[BillsPC] Created empty bills_pc.json");
        }
    }
    return true;
}

bool BillsPCStorageManager::readDocument(JsonDocument& doc) {
    if (!LittleFS.exists(BILLS_PC_FILE)) {
        doc.to<JsonArray>();
        return true;
    }

    File file = LittleFS.open(BILLS_PC_FILE, "r");
    if (!file) {
        Serial.println("[BillsPC] Error opening bills_pc.json for read");
        return false;
    }

    DeserializationError err = deserializeJson(doc, file);
    file.close();

    if (err) {
        Serial.printf("[BillsPC] Deserialization failed: %s\n", err.c_str());
        doc.to<JsonArray>();
        return false;
    }

    if (!doc.is<JsonArray>()) {
        doc.to<JsonArray>();
    }
    return true;
}

bool BillsPCStorageManager::writeDocument(const JsonDocument& doc) {
    // Atomic write using a temporary file
    File tempFile = LittleFS.open("/bills_pc.tmp", "w");
    if (!tempFile) {
        Serial.println("[BillsPC] Error opening temp file for write");
        return false;
    }

    if (serializeJson(doc, tempFile) == 0) {
        Serial.println("[BillsPC] Failed to write JSON to temp file");
        tempFile.close();
        LittleFS.remove("/bills_pc.tmp");
        return false;
    }
    tempFile.close();

    if (LittleFS.exists(BILLS_PC_FILE)) {
        LittleFS.remove(BILLS_PC_FILE);
    }

    if (!LittleFS.rename("/bills_pc.tmp", BILLS_PC_FILE)) {
        Serial.println("[BillsPC] Failed to rename temp file to bills_pc.json");
        return false;
    }

    return true;
}

bool BillsPCStorageManager::saveReceivedPokemon(PokemonData* pdata) {
    if (!pdata) return false;

    JsonDocument doc;
    readDocument(doc);
    JsonArray array = doc.as<JsonArray>();

    // Limit maximum stored entries if needed
    if (array.size() >= BILLS_PC_MAX_ENTRIES) {
        array.remove(0); // Remove oldest
    }

    JsonObject p = array.add<JsonObject>();

    // Generate unique ID
    char idBuf[32];
    snprintf(idBuf, sizeof(idBuf), "pkmn_%lu_%u", (unsigned long)millis(), (unsigned int)random(1000, 9999));
    p["id"] = idBuf;

    uint8_t species = pokemon_stat_get(pdata, STAT_NUM, NONE);
    p["species"] = species + 1; // 1-based dex
    p["name"] = table_stat_name_get(pdata->pokemon_table, species);
    p["level"] = pokemon_stat_get(pdata, STAT_LEVEL, NONE);
    p["origin_gen"] = (int)pdata->gen;
    p["target_gen"] = (int)pdata->gen;

    char nick[LEN_NICKNAME] = {0};
    pokemon_name_get(pdata, STAT_NICKNAME, nick, sizeof(nick));
    p["nickname"] = nick;

    char trainer[LEN_OT_NAME] = {0};
    pokemon_name_get(pdata, STAT_TRAINER_NAME, trainer, sizeof(trainer));
    p["trainer_name"] = trainer;

    char ot[LEN_OT_NAME] = {0};
    pokemon_name_get(pdata, STAT_OT_NAME, ot, sizeof(ot));
    p["ot_name"] = ot;

    p["ot_id"] = pokemon_stat_get(pdata, STAT_OT_ID, NONE);
    p["shiny"] = pokemon_is_shiny(pdata);
    p["item"] = (pdata->gen == GEN_II) ? pokemon_stat_get(pdata, STAT_HELD_ITEM, NONE) : 0;

    JsonArray moves = p["moves"].to<JsonArray>();
    moves.add(pokemon_stat_get(pdata, STAT_MOVE, MOVE_0));
    moves.add(pokemon_stat_get(pdata, STAT_MOVE, MOVE_1));
    moves.add(pokemon_stat_get(pdata, STAT_MOVE, MOVE_2));
    moves.add(pokemon_stat_get(pdata, STAT_MOVE, MOVE_3));

    JsonObject ivs = p["ivs"].to<JsonObject>();
    ivs["hp"] = pokemon_stat_get(pdata, STAT_HP_IV, NONE);
    ivs["atk"] = pokemon_stat_get(pdata, STAT_ATK_IV, NONE);
    ivs["def"] = pokemon_stat_get(pdata, STAT_DEF_IV, NONE);
    ivs["spd"] = pokemon_stat_get(pdata, STAT_SPD_IV, NONE);
    ivs["spc"] = (pdata->gen == GEN_I) ? pokemon_stat_get(pdata, STAT_SPC_IV, NONE) : pokemon_stat_get(pdata, STAT_SPC_ATK_IV, NONE);

    JsonObject evs = p["evs"].to<JsonObject>();
    evs["hp"] = pokemon_stat_get(pdata, STAT_HP_EV, NONE);
    evs["atk"] = pokemon_stat_get(pdata, STAT_ATK_EV, NONE);
    evs["def"] = pokemon_stat_get(pdata, STAT_DEF_EV, NONE);
    evs["spd"] = pokemon_stat_get(pdata, STAT_SPD_EV, NONE);
    evs["spc"] = pokemon_stat_get(pdata, STAT_SPC_EV, NONE);

    p["created_at"] = (unsigned long)millis();

    bool ok = writeDocument(doc);
    if (ok) {
        Serial.printf("[BillsPC] Saved received Pokémon %s (#%d, Lv.%d, Origin Gen %d)\n",
                      nick, species + 1, (int)p["level"], (int)p["origin_gen"]);
    }
    return ok;
}

String BillsPCStorageManager::getAllAsJson() {
    JsonDocument doc;
    readDocument(doc);
    String output;
    serializeJson(doc, output);
    return output;
}

bool BillsPCStorageManager::getPokemonJson(const String& id, JsonDocument& outDoc) {
    JsonDocument doc;
    readDocument(doc);
    JsonArray array = doc.as<JsonArray>();
    for (JsonObject obj : array) {
        if (obj["id"] == id) {
            outDoc.set(obj);
            return true;
        }
    }
    return false;
}

bool BillsPCStorageManager::updatePokemon(const String& id, JsonDocument& updatedFields) {
    JsonDocument doc;
    readDocument(doc);
    JsonArray array = doc.as<JsonArray>();

    for (JsonObject obj : array) {
        if (obj["id"] == id) {
            if (!updatedFields["nickname"].isNull()) obj["nickname"] = updatedFields["nickname"];
            if (!updatedFields["level"].isNull()) obj["level"] = updatedFields["level"];
            if (!updatedFields["shiny"].isNull()) obj["shiny"] = updatedFields["shiny"];
            if (!updatedFields["item"].isNull()) obj["item"] = updatedFields["item"];

            if (updatedFields["moves"].is<JsonArray>()) {
                JsonArray newMoves = updatedFields["moves"].as<JsonArray>();
                JsonArray targetMoves = obj["moves"].to<JsonArray>();
                for (JsonVariant m : newMoves) {
                    targetMoves.add(m.as<uint8_t>());
                }
            }

            // Time Capsule check: if origin_gen is 1, but has item > 0 or move > 165, target_gen becomes 2
            int originGen = obj["origin_gen"] | 1;
            int targetGen = originGen;
            uint8_t item = obj["item"] | 0;
            if (originGen == 1) {
                if (item > 0) targetGen = 2;
                JsonArray moves = obj["moves"].as<JsonArray>();
                for (JsonVariant m : moves) {
                    if (m.as<uint8_t>() > 165) {
                        targetGen = 2;
                        break;
                    }
                }
            }
            obj["target_gen"] = targetGen;

            return writeDocument(doc);
        }
    }
    return false;
}

bool BillsPCStorageManager::deletePokemon(const String& id) {
    JsonDocument doc;
    readDocument(doc);
    JsonArray array = doc.as<JsonArray>();

    for (size_t i = 0; i < array.size(); i++) {
        if (array[i]["id"] == id) {
            array.remove(i);
            return writeDocument(doc);
        }
    }
    return false;
}

bool BillsPCStorageManager::clearAll() {
    JsonDocument doc;
    doc.to<JsonArray>();
    return writeDocument(doc);
}

bool BillsPCStorageManager::loadPokemonToTrade(const String& id) {
    JsonDocument doc;
    if (!getPokemonJson(id, doc)) {
        return false;
    }

    uint8_t species = (doc["species"] | 1) - 1; // 0-based
    uint8_t level = doc["level"] | 50;
    const char* nickname = doc["nickname"] | "";
    const char* ot_name = doc["ot_name"] | "DHNRQZ";
    uint16_t ot_id = doc["ot_id"] | 20487;

    uint8_t m0 = 0, m1 = 0, m2 = 0, m3 = 0;
    if (doc["moves"].is<JsonArray>()) {
        JsonArray moves = doc["moves"].as<JsonArray>();
        if (moves.size() > 0) m0 = moves[0];
        if (moves.size() > 1) m1 = moves[1];
        if (moves.size() > 2) m2 = moves[2];
        if (moves.size() > 3) m3 = moves[3];
    }

    bool shiny = doc["shiny"] | false;
    uint8_t item = doc["item"] | 0;
    EvIvPreset eviv = MAXIV_MAXEV;

    // Switch engine generation if needed according to target_gen
    uint8_t targetGen = doc["target_gen"] | (doc["origin_gen"] | 1);
    if (TradeEngine.getGeneration() != targetGen) {
        TradeEngine.setGeneration(targetGen, false);
    }

    TradeEngine.configureOutgoingPokemon(
        species, level, nickname, ot_name, ot_id,
        m0, m1, m2, m3, eviv, shiny, item
    );

    Serial.printf("[BillsPC] Loaded Pokémon %s (#%d) into TradeEngine (Gen %d)\n",
                  nickname, species + 1, (int)targetGen);
    return true;
}

bool BillsPCStorageManager::importJson(const String& jsonContent) {
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, jsonContent);
    if (err || !doc.is<JsonArray>()) {
        Serial.printf("[BillsPC] Import JSON failed: %s\n", err.c_str());
        return false;
    }
    return writeDocument(doc);
}
