#pragma once

#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>

#include "gb_trade_engine.h"

class WebServerManager {
public:
    WebServerManager();
    void begin();
    void broadcastStatus(GBVisualStatus status, const char* msg);
    void broadcastReceivedPokemon(PokemonData* pdata);
    void broadcastGeneration(uint8_t gen);

private:
    AsyncWebServer _server;
    AsyncWebSocket _ws;

    void setupRoutes();
    void setupWebSocket();
};

extern WebServerManager WebManager;
