## Why

When completing a trade from a Game Boy (Gen I or Gen II), the received Pokémon in the web portal "Recibido" tab displays the same Pokémon sent from the ESP32 rather than the incoming Pokémon from the Game Boy, due to an unintended struct copy that overwrites incoming data with outgoing data in `STATE_TRADE_DONE`. Additionally, on mobile devices (iOS / Safari / Captive Network Assistant) and certain desktop browsers, the HTML5 `<datalist>` move inputs do not consistently display dropdown selections upon typing or tapping the arrow.

## What Changes

- **Fix Traded Pokémon Capture**: In `STATE_TRADE_DONE`, ensure the selected incoming Pokémon (`_incoming_selected_index`) in `_received_pdata` is preserved at slot 0, and adopt it into `_pdata` so the ESP32 holds the received Pokémon with its exact moves, level, OT ID, IVs, and held item.
- **WebSocket Payload Enrichment**: Include `shiny` and `item` fields in `broadcastReceivedPokemon()` to accurately convey Gen II attributes to the web interface.
- **Native Select Dropdowns for Moves**: Replace `<input list="moves_list">` with standard `<select>` dropdowns for both outgoing (`m0`-`m3`) and received (`rec_m0`-`rec_m3`) moves, populated dynamically with integer move IDs.
- **UX Tab Auto-Switch**: Automatically switch the web UI to the "Recibido" tab upon receiving a `received_pokemon` WebSocket event and pre-select its received moves, level, and shiny status.

## Capabilities

### Modified Capabilities
- `pokemon-trading-engine`: Capturing the incoming Pokémon from the Game Boy party into `_received_pdata` slot 0 and adopting it into `_pdata` upon trade completion.
- `captive-portal-web-ui`: Presenting move selectors as standard dropdown `<select>` controls and automatically switching to the Received tab with incoming move selections upon trade completion.

## Impact
- `src/gb_trade_engine.cpp`: Trade completion handler in `STATE_TRADE_DONE`.
- `src/web_server.cpp`: `broadcastReceivedPokemon` JSON fields.
- `include/web_assets.h` and `html/index.html`: Move selector elements and WebSocket event listener.
