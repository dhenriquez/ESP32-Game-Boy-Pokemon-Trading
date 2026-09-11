## Why

When trading in Generation II, held items attached to Pokémon via the web portal can be lost or fail to appear on the Game Boy Color due to several compounding factors:
1. When modifying an outgoing Pokémon while the Game Boy is already seated at the Cable Club trade table, the Game Boy does not re-read party buffers (which are only transferred upon room entry or table re-entry).
2. Generation auto-detection currently reallocates a blank default Pokémon, discarding any previously configured held item and moves.
3. The web portal's "Recibido" tab ignores incoming held items and hardcodes `item: 0` when queueing a received Pokémon to be returned to the Game Boy.
4. The Gen II patch list routine does not account for the 2-byte Trainer ID offset, risking memory corruption if escape bytes are encountered.
5. On ESP32-S3 SuperMini hardware, GPIO 48 connects to an addressable WS2812 RGB LED; driving it as a simple digital output fails to display meaningful status, missing the opportunity to visually reflect the web portal's color-coded link states.

## What Changes

- **Gen II Held Item UI & Return Support**: Display the held item name and sprite in the "Recibido" tab; add an item selector to the return form; and pass the selected `item` in `returnModifiedPokemon()` to `/api/return_traded` instead of hardcoded 0.
- **Trade Table Synchronization Notice**: Provide a clear UI notification/banner when a Pokémon is loaded while the Game Boy is already in `STATE_TRADE_SELECT` or `STATE_TRADE_PENDING`, informing the user to cancel (button B) on the Game Boy and re-sit at the table so the console downloads the fresh trade block.
- **Non-Destructive Generation Switching**: Update `GBTradeEngine::setGeneration()` to preserve existing configured species, nickname, moves, and held item when transitioning between generations, rather than wiping to an empty Bulbasaur.
- **Gen II Patch List Alignment**: In `src/patch_list.cpp`, account for the 2-byte `trainer_id` prefix in Gen II trade blocks so that `0xFE` escape bytes map to the correct memory offsets in Game Boy RAM.
- **RGB Status LED on GPIO 48**: Control the onboard WS2812 RGB LED on GPIO 48 via `rgbLedWrite(PIN_STATUS_LED, r, g, b)` to project the exact status colors of the web dashboard (`STATUS_WAITING_GB` = Red, `STATUS_LINK_CONNECTED` = Yellow, `STATUS_IN_TRADE_ROOM` = Blue, `STATUS_TRADE_SUCCESS` = Green, etc.).

## Capabilities

### Modified Capabilities

- `captive-portal-web-ui`: Display received held items, support held item selection when returning Pokémon, and alert the user to re-sit at the trade table if configuring while already seated.
- `pokemon-trading-engine`: Preserve configured Pokémon data across generation auto-detection, align Gen II patch list offsets, and drive the onboard RGB LED on GPIO 48 to match the link status colors.

## Impact

- `include/web_assets.h` and `html/index.html`: Received tab item display, item selector in return form, and table sync alert.
- `src/web_server.cpp`: Parse and apply `item` in `/api/return_traded`.
- `src/gb_trade_engine.cpp`: Preserve outgoing Pokémon across generation switch; expose table state.
- `src/patch_list.cpp`: Adjust Gen II patch offset by 2 bytes.
- `src/main.cpp`: Implement `setStatusLedColor()` using native `rgbLedWrite()`.
