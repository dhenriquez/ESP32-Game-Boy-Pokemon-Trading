## 1. Web Portal Held Item & Table Sync UI

- [x] 1.1 Add held item display to `#receivedData` summary and add `<select id="rec_item">` dropdown to "Recibido" tab in `include/web_assets.h` and `html/index.html`.
- [x] 1.2 Update `handleWSMessage` in `include/web_assets.h` to populate the received held item and pre-select it in `rec_item`.
- [x] 1.3 Update `returnModifiedPokemon()` in `include/web_assets.h` to pass `item` to `/api/return_traded`.
- [x] 1.4 Update `/api/configure` response and frontend toast/alert to detect if Game Boy is at the trade table and warn the user to re-seat the Game Boy to download fresh data.

## 2. Web Server & Trade Engine Backend

- [x] 2.1 Update `/api/return_traded` in `src/web_server.cpp` to parse `doc["item"]` and pass it to `TradeEngine.prepareReturnTradedPokemon()`.
- [x] 2.2 Expose `TradeEngine.isAtTradeTable()` in `src/gb_trade_engine.cpp` and include `in_table` status in `/api/configure` JSON response.
- [x] 2.3 Implement non-destructive generation switching in `GBTradeEngine::setGeneration()` in `src/gb_trade_engine.cpp`.
- [x] 2.4 Align Gen II patch list offset in `src/patch_list.cpp` by including the 2-byte Trainer ID offset.

## 3. GPIO 48 WS2812 RGB Status LED

- [x] 3.1 Implement `setStatusLedColor(GBVisualStatus status)` in `src/main.cpp` using `rgbLedWrite(PIN_STATUS_LED, r, g, b)` (or `neopixelWrite`).
- [x] 3.2 Connect `onTradeEngineEvent` callback to `setStatusLedColor` so the physical LED mirrors the web dashboard's visual status colors.

## 4. Verification & Testing

- [x] 4.1 Build the project with PlatformIO (`pio run`) to verify clean compilation.
- [x] 4.2 Upload firmware to the ESP32-S3 SuperMini and verify GPIO 48 RGB LED color transitions and Gen II held item behavior with Game Boy Color.
