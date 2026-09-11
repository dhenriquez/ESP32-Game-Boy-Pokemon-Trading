## Purpose

Provides an autonomous Wi-Fi SoftAP and Captive Portal web interface for customizing, managing, inspecting, and monitoring Pokémon trades on mobile devices and browsers.

## ADDED Requirements

### Requirement: Wi-Fi SoftAP and Captive Portal Redirection
The system SHALL broadcast a wireless Access Point and run an embedded DNS server that intercepts all domain requests and redirects clients to the local trading web application.

#### Scenario: Client connects to Access Point
- **WHEN** a client device connects to the `GB-PokeTrader` Wi-Fi network
- **THEN** the DNS server captures any operating system probe request and redirects the browser directly to `http://192.168.4.1`

### Requirement: Pokémon Generation and Customization Interface
The system SHALL provide an interactive single-page web application allowing the user to select and configure all attributes of the Pokémon to be sent.

#### Scenario: Customizing a Pokemon
- **WHEN** a user selects a species and modifies fields (level, moveset, nickname, OT name, OT ID, IV/EV preset)
- **THEN** the web application validates the inputs and updates the live trade block queued for the Game Boy

#### Scenario: Selecting species with automatic nickname assignment
- **WHEN** a user changes the species dropdown selector
- **THEN** the system automatically updates the Mote (Nickname) input with the uppercase name of the selected species (up to 10 characters), reloads the corresponding sprite, and updates the preview card

#### Scenario: Filtering species by active generation
- **WHEN** the user sets the generation to Gen I (Kanto)
- **THEN** the species dropdown only displays the first 151 Pokémon (Bulbasaur to Mew), and when switched to Gen II (Johto) it extends to all 251 Pokémon

#### Scenario: Selecting Gen II held item via dropdown
- **WHEN** the user enables Gen II options and chooses an item from the held items dropdown
- **THEN** the selected item is mapped to its internal ID and sent in the trade block payload

#### Scenario: Applying quick presets
- **WHEN** a user clicks a preset (such as trade evolution or legendary)
- **THEN** the form automatically populates with valid legal attributes for the chosen Pokémon, synchronizing the species dropdown, nickname, and item selector

### Requirement: Received Pokémon Inspection and Modification
The system SHALL display the party data received from the Game Boy and allow the user to modify its attributes before trading it back.

#### Scenario: Displaying received Pokemon
- **WHEN** the Game Boy transfers party data during trade room entry or trade completion
- **THEN** the web interface displays the received Pokémon's current level, stats, moves, and held item

#### Scenario: Modifying and queueing back
- **WHEN** the user modifies attributes (e.g., changes IVs, sets shininess, or alters moves) and clicks "Preparar para devolver"
- **THEN** the updated data is re-encoded into the trade block buffer ready for the next trade exchange

### Requirement: Real-Time Trade Status and Monitoring
The system SHALL establish a WebSocket connection between the web client and the ESP32 to provide live feedback on Game Boy cable communication.

#### Scenario: Live connection status transition
- **WHEN** the Game Boy changes link state (connected, trade room, trading, completed, disconnected)
- **THEN** the web interface updates the visual status indicators and logs the event in real time without requiring a page reload
