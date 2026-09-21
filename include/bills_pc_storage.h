#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "pokemon_data.h"

#define BILLS_PC_FILE "/bills_pc.json"
#define BILLS_PC_MAX_ENTRIES 100

class BillsPCStorageManager {
public:
    BillsPCStorageManager();

    bool begin();
    bool saveReceivedPokemon(PokemonData* pdata);
    String getAllAsJson();
    bool getPokemonJson(const String& id, JsonDocument& outDoc);
    bool updatePokemon(const String& id, JsonDocument& doc);
    bool deletePokemon(const String& id);
    bool clearAll();
    bool loadPokemonToTrade(const String& id);
    bool importJson(const String& jsonContent);

private:
    bool readDocument(JsonDocument& doc);
    bool writeDocument(const JsonDocument& doc);
};

extern BillsPCStorageManager BillsPC;
