## Context

The captive portal served by the ESP32-S3 allows users to configure Pokémon trades for Generation I and Generation II Game Boy games. Currently, species and items are handled by dynamic `<select>` elements, but the four moves (`m0` through `m3` and `rec_m0` through `rec_m3`) remain raw numeric inputs.

The firmware already stores a complete alphabetical list of 252 moves with their hex IDs and generation flags (`src/move_nl.cpp`). Providing a type-to-search interface that matches move names to internal IDs will improve usability and prevent invalid moves from breaking Time Capsule trades.

## Goals / Non-Goals

**Goals:**
- Provide type-to-search dropdown functionality for all 4 move inputs in both outgoing and received Pokémon tabs.
- Use English names matching the official Pokémon games and `src/move_nl.cpp`.
- Support filtering by Generation (165 Gen I moves when Gen I is active; 252 moves when Gen II is active).
- Ensure bidirectional mapping: resolve move names to uint8 byte IDs for the API, and resolve uint8 IDs to move names for presets and received trades.
- Keep the bundle lightweight and offline, running purely on native browser APIs with no CDN or third-party dependencies.

**Non-Goals:**
- Validating whether a specific Pokémon species can legally learn a move in the original games (e.g. standard trade editors allow custom movesets).
- Adding complex multi-column dropdowns with move stats (Power, Accuracy, PP).

## Decisions

### Decision 1: Use HTML5 `<datalist>` over custom JavaScript modal
- **Chosen Option:** HTML5 `<input type="text" list="moves_list">` paired with a single shared `<datalist id="moves_list">`.
- **Rationale:** Native support across iOS Safari, Android Chrome, and desktop browsers. All 8 input fields (`m0`..`m3`, `rec_m0`..`rec_m3`) share the exact same DOM `<datalist>`, requiring only ~4-5 KB of JSON/DOM representation in flash.
- **Alternatives Considered:**
  - *Custom Vanilla JS dropdown/modal:* Provides custom badge styling, but requires significantly more touch/scroll/focus management code on mobile devices.
  - *Standard `<select>`:* Does not support text search on mobile browsers (opens native scroll wheel).

### Decision 2: Compact Move Data Structure in Client Assets
- **Chosen Option:** Define `const ALL_MOVES = [{ name: "Pound", id: 1, gen: 1 }, ...];` in `html/index.html` (and `include/web_assets.h`).
- **Rationale:** Enables instant local filtering and bidirection lookup (`idToMoveName` and `moveNameToId`) without making HTTP calls to the ESP32.

### Decision 3: Preserving Backend API Contract
- **Chosen Option:** The `/api/configure` and `/api/return_traded` endpoints continue receiving integers `m0`..`m3`.
- **Rationale:** The frontend translates the input value (either a move name or an ID) to the appropriate numeric ID before JSON serialization.

## Risks / Trade-offs

- **[Freeform input by user]** → If the user types an unrecognized move name or a manual number, the parser will attempt a numeric parse or default to `0x00` (No Move).
- **[Generation mismatch]** → When switching from Gen II to Gen I, if any selected move is Gen II exclusive, the input is highlighted or reset to "No Move" / 0 to prevent Time Capsule rejection.
