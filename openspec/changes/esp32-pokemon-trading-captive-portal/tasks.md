# Tasks: ESP32 Game Boy Pokemon Trading with Captive Portal

## 1. PlatformIO Project Configuration & Hardware Setup

- [x] 1.1 Create `platformio.ini` configured for ESP32-S3 (SuperMini / devkitc-1) with Arduino framework, 4MB/8MB Flash settings, and library dependencies (`ESPAsyncWebServer-esphome`, `AsyncTCP-esphome`, `DNSServer`, `ArduinoJson`), verifying configuration structure.
- [x] 1.2 Create `include/hardware_pins.h` defining default GPIO pin assignments for ESP32-S3 SuperMini (CLK, SO, SI, and status LED), verifying header compiles cleanly.

## 2. Game Boy Data Structures & Protocol Engine (Port from Flipper Zero)

- [x] 2.1 Port Pokémon character encoding/decoding (`include/pokemon_char_encode.h` and `src/pokemon_char_encode.cpp`) and verify ASCII to Game Boy character table conversions.
- [x] 2.2 Port Pokémon tables and data structures (`include/pokemon_table.h`, `include/pokemon_data.h`, `src/pokemon_table.cpp`, `src/pokemon_data.cpp`) supporting 151 Gen I and 251 Gen II Pokémon with move sets, stats, and trade block formatting.
- [x] 2.3 Port patch list generator (`include/patch_list.h` and `src/patch_list.cpp`) handling `0xFE` byte escaping and verifying patch list reconstruction logic.
- [x] 2.4 Implement Game Boy Link SPI Slave driver (`include/gb_link_driver.h` and `src/gb_link_driver.cpp`) using high-speed GPIO edge interrupts on FreeRTOS Core 1.
- [x] 2.5 Implement Trade Center state machine (`include/gb_trade_engine.h` and `src/gb_trade_engine.cpp`) replicating Flipper Zero's trade states (`INIT`, `RANDOM`, `DATA`, `PATCH`, `SELECT`, `CONFIRMATION`, `DONE`).

## 3. Captive Portal, Web Server & WebSocket Synchronization

- [x] 3.1 Implement Wi-Fi SoftAP and Captive Portal DNS server (`include/wifi_portal.h` and `src/wifi_portal.cpp`) capturing DNS requests on port 53 and redirecting to `192.168.4.1`.
- [x] 3.2 Implement AsyncWebServer and WebSockets (`include/web_server.h` and `src/web_server.cpp`) streaming real-time trade engine events and handling Pokémon configuration payloads, serving embedded sprites from PROGMEM under `/sprites/{num}.png`.
- [x] 3.3 Create embedded Single Page Web Application (`include/web_assets.h`) and HTML preview (`html/index.html`):
  - Generator/Injector tab for creating/selecting Pokémon and presets (trade evolutions, starters, legendaries, event Mew/Celebi).
  - Receiver/Modifier tab for inspecting and modifying Pokémon received from the Game Boy.
  - Authentic 56x56 monochrome sprites sourced directly from `Flipper-Zero-Game-Boy-Pokemon-Trading/sprites/` (`scripts/generate_sprites.py`).
  - Real-time status indicator and console log driven by WebSockets.
  - Pinout configuration tab.

## 4. System Integration & Verification

- [x] 4.1 Implement `src/main.cpp` initializing Core 0 (Network & Web Server) and Core 1 (GB Trade Engine) isolated tasks with thread-safe mutex and queue communication.
- [x] 4.2 Compile firmware using PlatformIO and verify clean build with zero link or compiler errors.
- [x] 4.3 Update repository `README.md` with complete wiring instructions for ESP32-S3 SuperMini to Game Boy Link Cable, level shifter schematic, and user guide.
