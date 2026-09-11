## Context

In `GBTradeEngine`, incoming trade party data from the Game Boy is received into `_received_pdata->trade_block`. In `STATE_TRADE_DONE`, an erroneous copy (`pokemon_stat_memcpy(_received_pdata, _pdata, 0)`) was destroying this data by overwriting `_received_pdata` with the ESP32's outgoing `_pdata`. Furthermore, on mobile Safari and iOS Captive Portal browsers, `<datalist>` elements do not render options dropdowns reliably when tapped or typed, hindering move selection.

## Goals / Non-Goals

**Goals:**
- Correct the buffer management in `STATE_TRADE_DONE` so `_received_pdata` slot 0 receives the Pokémon at `_incoming_selected_index`, and `_pdata` adopts it with its exact moves, level, OT, IVs, and held item.
- Enrich `broadcastReceivedPokemon()` to include `shiny` and `item`.
- Replace move inputs and `<datalist>` with standard `<select>` dropdowns using integer move IDs for robust multi-platform behavior.
- Enhance UX to automatically switch to the "Recibido" tab upon receiving incoming Pokémon.

**Non-Goals:**
- Redesigning the underlying Pokémon data structures or patch list protocol.
- Changing pinout or hardware SPI configuration.

## Decisions

### 1. Memory Transfer in `STATE_TRADE_DONE`
- When `_incoming_selected_index != 0`, execute `pokemon_stat_memcpy(_received_pdata, _received_pdata, _incoming_selected_index)` to place the selected party member into slot 0.
- Execute `pokemon_stat_memcpy(_pdata, _received_pdata, 0)` to adopt the incoming Pokémon into `_pdata`.
- Set `_rebuild_patch_pending = true` and `_received_pending = true` to dispatch rebuilds and broadcasts in task context.

### 2. Native `<select>` Elements for Moves
- Replace `<input list="moves_list">` with `<select id="m0">`, `m1`, `m2`, `m3`, `rec_m0`, `rec_m1`, `rec_m2`, `rec_m3`.
- Dynamically populate options in `populateMovesSelect(gen)` with `<option value="id">Name (#id)</option>`.
- Read and write integer IDs directly (`parseInt(sel.value)`), removing fragile name-to-ID parsing functions.

### 3. Automatic Tab Switch on Trade Completion
- In `handleWSMessage`, when `data.type === 'received_pokemon'`, invoke `switchTab('tab-receive', document.querySelectorAll('.tab-btn')[1])`.
- Pre-select `rec_m0` through `rec_m3` using `data.moves[i]`.
- Set `rec_shiny.checked` using `data.shiny`.

## Risks / Trade-offs

- **[Risk]** Memory overlap during intra-struct copy (`pokemon_stat_memcpy(_received_pdata, _received_pdata, which)`).
  - **Mitigation**: `which != 0` addresses non-overlapping party, nickname, and ot_name array elements. When `which == 0`, no copy is needed.
