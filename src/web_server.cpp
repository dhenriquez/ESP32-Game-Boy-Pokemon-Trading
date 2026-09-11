#include "web_server.h"
#include "web_assets.h"
#include "pokemon_sprites.h"
#include "logo_data.h"

WebServerManager WebManager;

WebServerManager::WebServerManager()
    : _server(80)
    , _ws("/ws")
{
}

void WebServerManager::begin() {
    setupWebSocket();
    setupRoutes();
    _server.begin();
}

void WebServerManager::setupWebSocket() {
    _ws.onEvent([this](AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
        if (type == WS_EVT_CONNECT) {
            JsonDocument doc;
            doc["type"] = "status";
            doc["status"] = (int)TradeEngine.getVisualStatus();
            doc["title"] = TradeEngine.getVisualStatusString();
            doc["desc"] = TradeEngine.getVisualStatusString();
            doc["connected"] = (TradeEngine.getVisualStatus() > STATUS_WAITING_GB);
            doc["gen"] = (int)TradeEngine.getGeneration();

            String output;
            serializeJson(doc, output);
            client->text(output);
        }
    });

    _server.addHandler(&_ws);
}

void WebServerManager::setupRoutes() {
    // Root UI (streamed directly from Flash PROGMEM without heap allocation)
    _server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/html", (const uint8_t*)INDEX_HTML, sizeof(INDEX_HTML) - 1);
    });

    // Logo image
    auto serveLogo = [](AsyncWebServerRequest* request) {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "image/png", LOGO_PNG, sizeof(LOGO_PNG));
        response->addHeader("Cache-Control", "public, max-age=86400");
        request->send(response);
    };
    _server.on("/logo.png", HTTP_GET, serveLogo);
    _server.on("/docs/images/logo.png", HTTP_GET, serveLogo);

    // Captive portal probes redirection
    auto captiveRedirect = [](AsyncWebServerRequest* request) {
        request->redirect("/");
    };

    _server.on("/generate_204", HTTP_GET, captiveRedirect);
    _server.on("/hotspot-detect.html", HTTP_GET, captiveRedirect);
    _server.on("/canonical.html", HTTP_GET, captiveRedirect);
    _server.on("/connecttest.txt", HTTP_GET, captiveRedirect);
    _server.on("/ncsi.txt", HTTP_GET, captiveRedirect);
    _server.on("/check_network_status.txt", HTTP_GET, captiveRedirect);

    // API: Generation switch
    _server.on("/api/generation", HTTP_POST, [](AsyncWebServerRequest* request) {}, NULL,
        [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
            JsonDocument doc;
            DeserializationError err = deserializeJson(doc, data, len);
            if (!err) {
                uint8_t gen = doc["gen"] | 1;
                TradeEngine.setGeneration(gen);
                request->send(200, "application/json", "{\"success\":true}");
            } else {
                request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
            }
        }
    );

    // API: Configure outgoing Pokemon
    _server.on("/api/configure", HTTP_POST, [](AsyncWebServerRequest* request) {}, NULL,
        [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
            JsonDocument doc;
            DeserializationError err = deserializeJson(doc, data, len);
            if (!err) {
                uint8_t species = doc["species"] | 0;
                uint8_t level = doc["level"] | 50;
                const char* nickname = doc["nickname"] | "";
                const char* ot_name = doc["ot_name"] | "ESP32";
                uint16_t ot_id = doc["ot_id"] | 42069;
                uint8_t m0 = doc["m0"] | 0;
                uint8_t m1 = doc["m1"] | 0;
                uint8_t m2 = doc["m2"] | 0;
                uint8_t m3 = doc["m3"] | 0;
                EvIvPreset eviv = (EvIvPreset)(doc["eviv"] | 5);
                bool shiny = doc["shiny"] | false;
                uint8_t item = doc["item"] | 0;

                TradeEngine.configureOutgoingPokemon(
                    species, level, nickname, ot_name, ot_id,
                    m0, m1, m2, m3, eviv, shiny, item
                );

                bool in_table = TradeEngine.isAtTradeTable();
                char resp[64];
                snprintf(resp, sizeof(resp), "{\"success\":true,\"in_table\":%s}", in_table ? "true" : "false");
                request->send(200, "application/json", resp);
            } else {
                request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
            }
        }
    );

    // API: Return traded Pokemon with modifications
    _server.on("/api/return_traded", HTTP_POST, [](AsyncWebServerRequest* request) {}, NULL,
        [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
            JsonDocument doc;
            DeserializationError err = deserializeJson(doc, data, len);
            if (!err) {
                uint8_t level = doc["level"] | 50;
                const char* nickname = doc["nickname"] | "";
                uint8_t m0 = doc["m0"] | 0;
                uint8_t m1 = doc["m1"] | 0;
                uint8_t m2 = doc["m2"] | 0;
                uint8_t m3 = doc["m3"] | 0;
                bool shiny = doc["shiny"] | false;
                uint8_t item = doc["item"] | 0;

                TradeEngine.prepareReturnTradedPokemon(
                    level, nickname, m0, m1, m2, m3, shiny, item
                );

                bool in_table = TradeEngine.isAtTradeTable();
                char resp[64];
                snprintf(resp, sizeof(resp), "{\"success\":true,\"in_table\":%s}", in_table ? "true" : "false");
                request->send(200, "application/json", resp);
            } else {
                request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
            }
        }
    );

    // Catch-all handler: serves embedded sprites or redirects to root for captive portal
    _server.onNotFound([](AsyncWebServerRequest* request) {
        String url = request->url();
        if (url.startsWith("/sprites/")) {
            int slash = url.lastIndexOf('/');
            int dot = url.lastIndexOf('.');
            if (slash >= 0 && dot > slash) {
                uint16_t num = url.substring(slash + 1, dot).toInt();
                EmbeddedSprite sp = get_embedded_sprite(num);
                if (sp.data && sp.size > 0) {
                    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/png", sp.data, sp.size);
                    response->addHeader("Cache-Control", "public, max-age=86400");
                    request->send(response);
                    return;
                }
            }
            request->send(404, "text/plain", "Sprite not found");
            return;
        }
        request->redirect("/");
    });
}

void WebServerManager::broadcastStatus(GBVisualStatus status, const char* msg) {
    if (_ws.count() == 0) return;

    JsonDocument doc;
    doc["type"] = "status";
    doc["status"] = (int)status;
    doc["title"] = TradeEngine.getVisualStatusString();
    doc["desc"] = msg ? msg : TradeEngine.getVisualStatusString();
    doc["connected"] = (status > STATUS_WAITING_GB);

    String output;
    serializeJson(doc, output);
    _ws.textAll(output);
}

void WebServerManager::broadcastReceivedPokemon(PokemonData* pdata) {
    if (!pdata || _ws.count() == 0) return;

    JsonDocument doc;
    doc["type"] = "received_pokemon";
    uint8_t species = pokemon_stat_get(pdata, STAT_NUM, NONE);
    doc["species"] = species + 1;
    doc["name"] = table_stat_name_get(pdata->pokemon_table, species);
    doc["level"] = pokemon_stat_get(pdata, STAT_LEVEL, NONE);
    doc["shiny"] = pokemon_is_shiny(pdata);
    doc["item"] = (pdata->gen == GEN_II) ? pokemon_stat_get(pdata, STAT_HELD_ITEM, NONE) : 0;

    char nick[LEN_NICKNAME];
    pokemon_name_get(pdata, STAT_NICKNAME, nick, sizeof(nick));
    doc["nickname"] = nick;

    JsonArray moves = doc["moves"].to<JsonArray>();
    moves.add(pokemon_stat_get(pdata, STAT_MOVE, MOVE_0));
    moves.add(pokemon_stat_get(pdata, STAT_MOVE, MOVE_1));
    moves.add(pokemon_stat_get(pdata, STAT_MOVE, MOVE_2));
    moves.add(pokemon_stat_get(pdata, STAT_MOVE, MOVE_3));

    String output;
    serializeJson(doc, output);
    _ws.textAll(output);
}

void WebServerManager::broadcastGeneration(uint8_t gen) {
    if (_ws.count() == 0) return;

    JsonDocument doc;
    doc["type"] = "generation";
    doc["gen"] = (int)gen;

    String output;
    serializeJson(doc, output);
    _ws.textAll(output);
}

