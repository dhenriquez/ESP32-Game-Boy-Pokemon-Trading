## 1. Trade Engine Received Pokémon Fix

- [x] 1.1 Correct `STATE_TRADE_DONE` in `src/gb_trade_engine.cpp` to copy incoming Pokémon to slot 0 of `_received_pdata` and adopt it into `_pdata`.
- [x] 1.2 Enrich `WebServerManager::broadcastReceivedPokemon()` in `src/web_server.cpp` with `shiny` and `item` fields.

## 2. Web UI Native Selects for Moves and UX Auto-Switch

- [x] 2.1 Replace move `<input list="moves_list">` elements with native `<select>` dropdowns in `include/web_assets.h` and `html/index.html`.
- [x] 2.2 Implement `populateMovesSelect(gen)` in `include/web_assets.h` and `html/index.html` to populate move dropdowns with integer IDs and names.
- [x] 2.3 Update presets, `savePokemon()`, and `returnModifiedPokemon()` in `include/web_assets.h` and `html/index.html` to use integer values directly.
- [x] 2.4 Update `handleWSMessage` in `include/web_assets.h` to auto-switch to "Recibido" tab and populate received moves, level, and shiny status.

## 3. Compilation and Upload

- [x] 3.1 Compile firmware using PlatformIO (`pio run`).
- [x] 3.2 Upload firmware to ESP32-S3 SuperMini and verify successful boot and trading behavior.
