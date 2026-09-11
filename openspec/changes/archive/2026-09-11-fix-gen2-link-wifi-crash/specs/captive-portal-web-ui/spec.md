## MODIFIED Requirements

### Requirement: Real-Time Trade Status and Monitoring
The system SHALL establish a WebSocket connection between the web client and the ESP32 to provide live feedback on Game Boy cable communication.

#### Scenario: Live connection status transition
- **WHEN** the Game Boy changes link state (connected, trade room, trading, completed, disconnected)
- **THEN** the web interface updates the visual status indicators and logs the event in real time without requiring a page reload

#### Scenario: Live generation auto-synchronization
- **WHEN** the ESP32 broadcasts a `generation` event detected from the Game Boy Link connection
- **THEN** the web application updates its internal generation state, updates the generation selector and badge, and dynamically adjusts available species, items, and moves without dropping the connection
