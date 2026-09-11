# Proposal: ESP32 Game Boy Pokemon Trading with Captive Portal

## Why

The original Flipper Zero Pokemon Trading application provides a great way to trade and modify Gen I and Gen II Pokémon with physical Game Boy hardware, but it requires a Flipper Zero device (~$169 USD). Porting this functionality to an ESP32 (such as the compact ESP32-S3 SuperMini) paired with a Wi-Fi Captive Portal makes the tool ultra-affordable, accessible, and user-friendly: any player can connect with their smartphone, customize or modify Pokémon through an intuitive web interface, and trade directly with an original Game Boy over a Link Cable.

## What Changes

- **PlatformIO Setup**: Create a PlatformIO C++ project with the Arduino framework configured for ESP32-S3 (and compatible ESP32 targets), using FreeRTOS for task and core isolation.
- **Game Boy Link Slave Engine**: Port the low-level SPI slave communication and trade protocol from `flipper-gblink` and `Flipper-Zero-Game-Boy-Pokemon-Trading` to run on a dedicated ESP32 core (Core 1) with microsecond-level timing and zero Wi-Fi jitter.
- **Pokémon Data & Trade Block Management**: Adapt Gen I and Gen II Pokémon data structures, Pokédex tables (151 Gen I + 251 Gen II), movesets, base stats, exp curves, patch list generation (handling `0xFE` byte escape), and character encoding routines.
- **Captive Portal & Network Stack**: Implement SoftAP Wi-Fi (`GB-PokeTrader`) with a DNS server (port 53) redirecting all traffic to `192.168.4.1`, triggering automatic captive portal popups on iOS, Android, and Windows.
- **Modern Responsive Web UI**: Build a single-page web application embedded in the ESP32 featuring:
  - **Generator / Injector Tab**: Select any Gen I / Gen II Pokémon, customize Level, Nickname, OT Details, Moves, EVs/IVs, Shininess, Held Items, and select from one-click presets (trade evolutions, starters, legendaries, event Mew/Celebi).
  - **Receiver / Modifier Tab**: Inspect Pokémon sent by the Game Boy, edit their stats or shininess, and queue them to trade back.
  - **Live Trade Monitor**: Real-time status display and logs via WebSockets (Idle, Handshake, Trade Center, Transferring, Complete).
  - **Pinout Settings**: Allow configurable GPIO pin assignments for CLK, SO, and SI.

## Capabilities

### New Capabilities
- `pokemon-trading-engine`: Low-level Game Boy Link Cable SPI Slave engine, Gen I & Gen II data tables, trade block formation, patch list generator, character encoding, and trade state machine pinned to Core 1.
- `captive-portal-web-ui`: Wi-Fi SoftAP, DNS captive portal redirection, asynchronous web server, WebSockets live connection, and responsive mobile-first Web UI for Pokémon customization and monitoring.

### Modified Capabilities
*(None - fresh project implementation)*

## Impact

- Adds new PlatformIO project files: `platformio.ini`, C/C++ source in `src/`, headers in `include/`, and web assets in `data/`.
- Hardware dependency: Requires ESP32-S3 / ESP32 connected to Game Boy Link Cable lines (CLK, SO, SI, GND) with appropriate voltage protection.
- No breaking changes or regressions to existing documentation in the repository.
