# Design: ESP32 Game Boy Pokemon Trading with Captive Portal

## Context

The system ports the Pokémon Gen I and Gen II trade protocol from the Flipper Zero implementation (`Flipper-Zero-Game-Boy-Pokemon-Trading`) to the ESP32 platform (specifically targeting the ESP32-S3 SuperMini). The device must act as an SPI Slave to the Game Boy Link Cable (~8 kHz clock rate) while simultaneously hosting a Wi-Fi Access Point, Captive Portal DNS server, and WebSocket-driven web application for user control.

## Goals / Non-Goals

**Goals:**
- Provide rock-solid ~8 kHz SPI slave response times (<15 µs bit setup time) by isolating the Game Boy link engine to FreeRTOS Core 1.
- Serve a fast, responsive, zero-internet-required Web UI (SPA) from ESP32 flash memory via SoftAP (`GB-PokeTrader`) and Captive Portal redirection.
- Support full Generation I (Red/Blue/Yellow) and Generation II (Gold/Silver/Crystal) international cartridge trades.
- Enable both injection of custom/preset Pokémon and reading/modification/re-trade of Pokémon sent from the Game Boy cartridge.
- Real-time WebSocket synchronization between the web client and trade engine.

**Non-Goals:**
- Supporting Japanese cartridge formats (which use different memory layouts).
- Colosseum / Battle mode emulation (scope is focused on the Cable Club Trade Center).
- Game Boy Printer emulation.

## Decisions

### Decision 1: PlatformIO with Arduino Framework and FreeRTOS
- **Choice:** C++ with Arduino Framework managed via PlatformIO.
- **Rationale:** MicroPython incurs garbage-collection pauses (2–15 ms) and interrupt dispatch overhead (50–100 µs) that cause bit drops and link desynchronization on the Game Boy's ~8 kHz clock. C++ on FreeRTOS delivers sub-microsecond ISR execution and true dual-core task affinity.
- **Alternatives considered:** MicroPython (rejected due to timing jitter and GC pauses); ESP-IDF native (viable, but Arduino framework provides faster iteration and vast ecosystem for `ESPAsyncWebServer`).

### Decision 2: Dual-Core Task and Memory Architecture
- **Choice:**
  - **Core 0**: Wi-Fi SoftAP, `DNSServer` on port 53 (Captive Portal), `ESPAsyncWebServer` on port 80, and WebSocket event dispatcher.
  - **Core 1**: Dedicated high-priority FreeRTOS task and GPIO ISR handling Game Boy clock transitions, bit shifting, and the trade state machine.
  - **Synchronization**: Thread-safe mutex-protected shared `TradeContext` buffer and FreeRTOS queues for state event notifications to WebSockets.
- **Rationale:** Wi-Fi radio interrupts on Core 0 will never interrupt or cause jitter on Core 1's link cable bit sampling.

```
+-------------------------------------------------------------------------+
| ESP32-S3 Dual-Core Architecture                                         |
|                                                                         |
|  [ CORE 0: Network & Web ]               [ CORE 1: GB Link Engine ]     |
|  +-------------------------+             +---------------------------+  |
|  | SoftAP "GB-PokeTrader"  |             | GPIO ISR (CLK, SO, SI)    |  |
|  | Captive Portal DNS (:53)|             | 8 kHz SPI Slave Shifter   |  |
|  | AsyncWebServer (:80)    |             | Trade State Machine       |  |
|  | WebSocket Server        |             | (RESET/INIT/DATA/CONFIRM) |  |
|  +-------------------------+             +---------------------------+  |
|               |                                        |                |
|               +---------> [ TradeContext Mutex ] <-----+                |
|                           [ FreeRTOS Event Queue ]                      |
+-------------------------------------------------------------------------+
```

### Decision 3: Self-Contained Web UI (Single Page App in Flash)
- **Choice:** Embed the HTML5, CSS3, and JavaScript Single Page App directly into firmware (using Gzip compression and flash PROGMEM storage).
- **Rationale:** Eliminates the need for external SD cards or SPIFFS filesystem flashing, making firmware flashing a single-step process. The UI loads instantly even on mobile captive portal mini-browsers.
- **Design Aesthetic:** Retro-modern Game Boy themed interface with clean cards, responsive touch controls, Pokémon sprites, and live status badges.

### Decision 4: Hardware Interface and Level Shifting
- **Choice:**
  - **GB CLK (Pin 5) -> ESP32 Input (e.g. GPIO 4)**: 5V to 3.3V via voltage divider (1kΩ / 2kΩ) or bidirectional level shifter.
  - **GB SO (Pin 2) -> ESP32 Input (e.g. GPIO 5)**: 5V to 3.3V via voltage divider (1kΩ / 2kΩ) or level shifter.
  - **ESP32 Output (e.g. GPIO 6) -> GB SI (Pin 3)**: Direct 3.3V logic (Game Boy reliably reads 3.3V as HIGH logic).
  - **GND (Pin 6) <-> ESP32 GND**: Common ground.
  - **ESP32 Power**: Powered via USB-C or external LiPo to avoid drawing current from the Game Boy's 5V rail.

### Decision 5: Client-Side Embedded Catalogs for Species and Items
- **Choice:** Embed the 251 Pokémon species names and the Gen II held item table (`item_nl.cpp`) directly in client JavaScript.
- **Rationale:** Embedding these compact lookup tables (~2.5 KB total) directly into the SPA HTML payload avoids additional HTTP/REST round-trips to the ESP32, eliminates network latency, and enables instantaneous dropdown filtering (151 vs 251 according to generation) and real-time auto-fill of nicknames upon species selection.

## Risks / Trade-offs

- **[Risk: 5V logic damage from Game Boy DMG/GBC]**  
  → *Mitigation:* Explicit pinout documentation and warnings instructing the user to use resistor dividers or level shifters on incoming 5V lines (CLK and SO).
- **[Risk: Trade desync if Web UI user edits data mid-trade]**  
  → *Mitigation:* The active `trade_block` is locked with a mutex once the Game Boy enters the `TRADE_INIT` state. Edits in the Web UI are queued to apply only when the trade engine is in `TRADE_RESET` or `WAITING` state.
- **[Risk: Mobile OS captive portal automatic disconnect]**  
  → *Mitigation:* Support both captive portal automatic popup and direct browser access at `192.168.4.1`, with persistent WebSocket auto-reconnect logic in JavaScript.
