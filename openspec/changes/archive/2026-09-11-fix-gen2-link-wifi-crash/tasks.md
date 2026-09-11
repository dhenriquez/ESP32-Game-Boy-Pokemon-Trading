## 1. Trade Engine ISR Decoupling and IRAM Placement

- [x] 1.1 Add volatile status flags (`_status_pending`, `_pending_status`, `_pending_msg`, `_pending_gen_switch`, `_received_pending`, `_rebuild_patch_pending`) and `process()` method to `include/gb_trade_engine.h`.
- [x] 1.2 Refactor `updateStatus()` in `src/gb_trade_engine.cpp` to set atomic flags without invoking callbacks or network functions from the ISR.
- [x] 1.3 Implement `GBTradeEngine::process()` in `src/gb_trade_engine.cpp` to safely dispatch status callbacks, received Pokémon events, and patch list rebuilds in task context.
- [x] 1.4 Add `IRAM_ATTR` attributes to `driver_callback_shim`, `onByteExchange`, `handleConnectPhase`, `handleMenuPhase`, `handleTradeCentrePhase`, and `plist_index_get`.

## 2. Automatic Generation Detection and Task Integration

- [x] 2.1 Implement automatic generation detection in `handleConnectPhase()` for `0x60` (Gen I) and `0x61` (Gen II), queuing a safe generation switch.
- [x] 2.2 Add `broadcastGeneration()` in `include/web_server.h` and `src/web_server.cpp` to broadcast generation changes over WebSocket.
- [x] 2.3 Integrate `TradeEngine.process()` into `main.cpp` in `loop()` and `networkTask`.

## 3. Web UI Synchronization

- [x] 3.1 Update WebSocket event handler in `include/web_assets.h` and `html/index.html` to dynamically update the generation selector and badge upon receiving `generation` events.

## 4. Compilation and Flash Verification

- [x] 4.1 Compile firmware using PlatformIO (`pio run`).
- [x] 4.2 Upload firmware to ESP32-S3 SuperMini and verify that connecting a Game Boy Color with Pokémon Gold maintains a stable Wi-Fi connection without drops.

