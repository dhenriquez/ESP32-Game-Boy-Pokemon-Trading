## Context

The ESP32-S3 SuperMini functions as an autonomous Game Boy Link Cable SPI Slave. While Generation II trades function mechanically, held items can fail to reach the Game Boy due to the single-pass nature of the Link Protocol's room entrance handshake, auto-detection state resets, and UI omission in the "Recibido" tab. Furthermore, GPIO 48 on the ESP32-S3 SuperMini is routed to an onboard addressable WS2812 RGB LED that currently only receives digital HIGH/LOW states instead of color-coded status indication.

## Goals / Non-Goals

**Goals:**
- Enable full held item inspection, editing, and return queueing in the web interface "Recibido" tab.
- Provide clear real-time user feedback when modifying a Pokémon while the Game Boy is already seated at the trade table.
- Make generation auto-detection non-destructive, preserving pre-configured Pokémon attributes across link handshake.
- Align Gen II patch list serialization with the 2-byte Trainer ID prefix in Game Boy WRAM.
- Drive the onboard WS2812 RGB LED on GPIO 48 to match the color palette of the web status badge and dot.

**Non-Goals:**
- Bypassing the Game Boy ROM's internal trade evolution item consumption (e.g. Scyther/Onix holding Metal Coat), which is hardcoded in the cartridge ROM.
- Introducing third-party RGB LED libraries; utilize the ESP32 Arduino Core's native `rgbLedWrite()` / `neopixelWrite()` functions.

## Decisions

### 1. RGB Status LED on GPIO 48 via `rgbLedWrite`
The ESP32-S3 Arduino core provides `rgbLedWrite(pin, r, g, b)` (and `neopixelWrite(pin, r, g, b)`) for driving onboard WS2812 LEDs without external library dependencies.
In `src/main.cpp`, we implement `setStatusLedColor(GBVisualStatus status)`:
- `STATUS_WAITING_GB`: Red `(64, 0, 0)`
- `STATUS_LINK_CONNECTED`: Amber/Yellow `(64, 32, 0)`
- `STATUS_IN_TRADE_ROOM`, `STATUS_SELECTING_POKEMON`, `STATUS_CONFIRMING_DEAL`: Blue `(0, 32, 64)`
- `STATUS_EXCHANGING_DATA`, `STATUS_TRADING_ANIMATION`: Purple/Magenta `(48, 0, 64)`
- `STATUS_TRADE_SUCCESS`: Green `(0, 64, 0)`

*Alternative considered:* Third-party library (`FastLED`). Rejected due to unnecessary bloat and core incompatibilities on ESP32-S3.

### 2. Trade Table Synchronization Detection and Guidance
In `GBTradeEngine`, `_trade_state` tracks when the Game Boy is seated at the table (`STATE_TRADE_SELECT` or `STATE_TRADE_PENDING`).
When `/api/configure` is called:
- If `TradeEngine.isAtTradeTable()` is true, `/api/configure` responds with `{"success": true, "in_table": true}`.
- The web frontend displays an actionable warning toast:
  *"⚠️ Pokémon y objeto cargados en memoria. Como tu Game Boy ya está en la mesa, pulsa B (Cancelar) en la consola y vuelve a interactuar con la mesa para descargar los datos actualizados."*

*Alternative considered:* Sending `table_leave` packet directly from ESP32. Rejected because interrupting the Game Boy's state machine without user input can trigger communication timeouts or link disconnect error screens on the console.

### 3. Non-Destructive Generation Switching
In `GBTradeEngine::setGeneration(uint8_t gen, bool reset_link)`:
- If `_pdata` is already populated with custom data (species != default Bulbasaur, custom nickname, or non-zero moves):
  - Extract the current attributes before freeing `_pdata`.
  - Allocate the new generation `_pdata`.
  - Re-apply the extracted attributes to the newly allocated structure.
- Prevents auto-detection of Gen II from wiping user-configured Pokémon.

### 4. Held Items in Recibido Tab & Return Handler
- In `web_assets.h` and `html/index.html`:
  - Add `<select id="rec_item">` to the edit form in Tab 2.
  - Update `handleWSMessage` to display the received item name on `#recSummaryDetails` and set `rec_item.value = data.item || 0`.
  - In `returnModifiedPokemon()`, pass `item: parseInt(document.getElementById('rec_item').value) || 0` in the payload.
- In `src/web_server.cpp`:
  - Update `/api/return_traded` to parse `doc["item"]` and forward it to `TradeEngine.prepareReturnTradedPokemon()`.

### 5. Gen II Patch List 2-Byte Offset Correction
In Pokémon Gold/Silver (`pokegold/engine/link/link.asm:L614`), the patch list targets `wLinkPlayerPatchedData`, which starts at `wLinkPlayerID` (2 bytes before `wLinkPlayerPartyMon1`).
In `src/patch_list.cpp`:
- If `pdata->gen == GEN_II`, include the 2-byte offset in the index calculation: `offset = i + 2`.
- Ensures that any `0xFE` escape bytes patch the exact intended RAM location on the Game Boy.

## Risks / Trade-offs

- **[Risk]** WS2812 RGB LED brightness causing excessive glare.
  - **Mitigation**: Scale RGB values to comfortable intensity levels (capped at ~64 out of 255).
- **[Risk]** Users missing the table synchronization warning.
  - **Mitigation**: Display the notice prominently in high-contrast amber styling and log it to the web console box.
