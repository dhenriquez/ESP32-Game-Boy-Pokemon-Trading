## Why

Currently, Pokémon moves in the captive portal interface are configured using plain numeric input fields (`m0` through `m3` for outgoing Pokémon, and `rec_m0` through `rec_m3` for received Pokémon). This forces users to know or externally look up the decimal/hex IDs of attacks (e.g., 85 for Thunderbolt, 138 for Dream Eater). Providing a searchable native dropdown with attack names in English makes move selection intuitive, prevents errors, and filters out Gen II moves when Gen I trading is active to prevent Time Capsule rejections.

## What Changes

- Convert move inputs (`m0`, `m1`, `m2`, `m3` and `rec_m0`...`rec_m3`) to text inputs linked to a shared HTML5 `<datalist id="moves_list">`.
- Embed the official 252 Pokémon moves from `src/move_nl.cpp` in English with their internal IDs and generation flags into the web UI client assets.
- Dynamically filter the moves list according to the selected Generation (165 legal moves for Gen I; all 252 moves for Gen II).
- Automatically resolve move names to their corresponding byte IDs when submitting to `/api/configure` or `/api/return_traded`.
- Automatically resolve move byte IDs to their official move names when loading presets or when inspecting a received Pokémon from the Game Boy.

## Capabilities

### New Capabilities
<!-- None -->

### Modified Capabilities
- `captive-portal-web-ui`: Add requirements for searchable attack dropdowns with generation-aware filtering, name-to-ID payload resolution, and preset/inspection move name synchronization.

## Impact

- Frontend web assets: `html/index.html` and `include/web_assets.h`.
- Backend endpoints: `/api/configure` and `/api/return_traded` continue receiving uint8 move IDs with no breaking API changes.
