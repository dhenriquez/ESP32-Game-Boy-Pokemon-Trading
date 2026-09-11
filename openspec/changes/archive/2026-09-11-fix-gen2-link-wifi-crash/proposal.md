## Why

When connecting a Game Boy Color running Pokémon Gold (Gen II) to the ESP32-S3 via the Link cable, the Access Point Wi-Fi connection drops immediately. This occurs because the hardware GPIO interrupt service routine (`GBLinkDriver::isrHandler`) calls `updateStatus()`, which invokes `WebManager.broadcastStatus()` and `_ws.textAll()`—triggering FreeRTOS mutexes and heap allocations (`malloc`) directly inside an ISR. Calling network socket writes and memory allocations from an ISR causes an immediate ESP-IDF kernel panic and CPU reset. Furthermore, the firmware boots into Gen I (415 bytes) by default, causing buffer overflows when Gen II (441 bytes + 389 bytes Mail) sends data before manual configuration.

## What Changes

- Decouple the hardware link ISR from network socket broadcasts using atomic state flags, deferring WebSocket transmissions and status broadcasts to the main task loop context (`TradeEngine.process()`).
- Implement automatic generation detection in the initial handshake: when the Game Boy sends `0x61` (`PKMN_CONNECTED_II`), the engine automatically switches to Gen II (441-byte buffers) and notifies the web client; when `0x60` is received, it operates in Gen I mode.
- Add `IRAM_ATTR` to all SPI link protocol execution routines to prevent cache-disabled memory access during Wi-Fi transmissions.
- Safely defer `rebuildPatchList()` (which invokes `malloc`/`free`) and received Pokémon struct cloning from the ISR to task context.
- Update the web interface to handle incoming `generation` WebSocket events and auto-switch the UI view when Gen II is detected.

## Capabilities

### New Capabilities
<!-- None -->

### Modified Capabilities
- `pokemon-trading-engine`: Add non-blocking ISR status signaling, task-level event processing, and automatic Gen I vs Gen II handshake detection.
- `captive-portal-web-ui`: Update WebSocket handler to support live generation synchronization when the Game Boy is connected.

## Impact

- Core firmware stability: Eliminates kernel panics and Wi-Fi disconnects when connecting a Game Boy.
- No breaking API changes; the web interface automatically synchronizes with the detected console.
