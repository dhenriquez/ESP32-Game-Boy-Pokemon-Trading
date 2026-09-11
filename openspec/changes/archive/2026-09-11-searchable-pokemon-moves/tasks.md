## 1. Move Dataset and UI Markup

- [x] 1.1 Extract the 252 moves from `src/move_nl.cpp` into a compact JavaScript array `ALL_MOVES` containing `name`, `id`, and `gen`.
- [x] 1.2 Add the shared `<datalist id="moves_list"></datalist>` to the web UI markup.
- [x] 1.3 Replace numeric attack inputs (`m0`..`m3` and `rec_m0`..`rec_m3`) with text inputs referencing `list="moves_list"`.

## 2. Dynamic Filtering and Value Resolution

- [x] 2.1 Implement `populateMovesDatalist(gen)` to filter between Gen I (165 moves) and Gen II (252 moves) based on the active generation.
- [x] 2.2 Implement bidirectional helpers `moveIdToName(id)` and `moveToId(inputVal)` for mapping names to uint8 byte IDs.
- [x] 2.3 Update `/api/configure` form submit handler to serialize attack names into byte IDs `m0`..`m3`.
- [x] 2.4 Update `/api/return_traded` handler to serialize received Pokémon attack names into byte IDs.

## 3. Presets and Received Trade Synchronization

- [x] 3.1 Update `applyPreset()` to populate attack fields with friendly move names instead of raw IDs.
- [x] 3.2 Update WebSocket party receive handler / preview to resolve incoming move IDs into English move names in `rec_m0`..`rec_m3`.
- [x] 3.3 Synchronize changes between `html/index.html` and `include/web_assets.h`.

## 4. Verification and Build

- [x] 4.1 Verify firmware compilation with PlatformIO (`pio run`).
- [x] 4.2 Verify that the web interface loads correctly without memory leaks or missing elements.
