#include <Arduino.h>

#include "hardware_pins.h"
#include "wifi_portal.h"
#include "web_server.h"
#include "gb_trade_engine.h"

// Task for Core 0 handling Web and DNS processing
void networkTask(void* parameter) {
    for (;;) {
        Portal.process();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void onTradeEngineEvent(GBVisualStatus status, const char* msg) {
    WebManager.broadcastStatus(status, msg);

    // If trade completed successfully, also broadcast received pokemon details
    if (status == STATUS_TRADE_SUCCESS) {
        WebManager.broadcastReceivedPokemon(TradeEngine.getReceivedPokemonData());
    }

    #if defined(PIN_STATUS_LED)
    if (status > STATUS_WAITING_GB) {
        digitalWrite(PIN_STATUS_LED, LOW); // LED ON (active low on many ESP32 boards)
    } else {
        digitalWrite(PIN_STATUS_LED, HIGH); // LED OFF
    }
    #endif
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n==========================================");
    Serial.println("  ESP32 Game Boy Pokemon Trading Station  ");
    Serial.println("==========================================");

    #if defined(PIN_STATUS_LED)
    pinMode(PIN_STATUS_LED, OUTPUT);
    digitalWrite(PIN_STATUS_LED, HIGH);
    #endif

    // Initialize Trade Engine and Link Driver
    TradeEngine.setEventCallback(onTradeEngineEvent);
    TradeEngine.begin(GEN_I, PIN_GB_CLK, PIN_GB_SO, PIN_GB_SI);
    Serial.printf("[GB Link] Initialized on CLK:%d, SO:%d, SI:%d\n", PIN_GB_CLK, PIN_GB_SO, PIN_GB_SI);

    // Initialize SoftAP & Captive Portal DNS on Core 0
    Portal.begin("GB-PokeTrader");
    Serial.printf("[Wi-Fi] SoftAP started. Connect to 'GB-PokeTrader'\n");
    Serial.printf("[Portal] IP: %s\n", Portal.getIP().toString().c_str());

    // Initialize AsyncWebServer & WebSockets
    WebManager.begin();
    Serial.println("[Web] Web server and WebSocket listening on port 80");

    // Launch background task on Core 0 for network & DNS handling
    xTaskCreatePinnedToCore(
        networkTask,
        "NetworkTask",
        4096,
        NULL,
        1,
        NULL,
        0
    );

    Serial.println("[System] Ready for trading!");
}

void loop() {
    // LED heartbeat / connection status blink
    static uint32_t last_blink = 0;
    if (millis() - last_blink > 500) {
        last_blink = millis();
        if (GBLink.isConnected()) {
            #if defined(PIN_STATUS_LED)
            digitalWrite(PIN_STATUS_LED, !digitalRead(PIN_STATUS_LED));
            #endif
        }
    }

    delay(100);
}
