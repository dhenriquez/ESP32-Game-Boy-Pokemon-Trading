## ADDED Requirements

### Requirement: Non-Destructive Generation Switching
The system SHALL preserve user-configured Pokémon attributes (species, nickname, moves, OT details, and held items) when the trading engine switches active generation during auto-detection.

#### Scenario: Switching from Gen I default to Gen II on console handshake
- **WHEN** the Game Boy initiates a Gen II connection (`0x61`) after the user has configured an outgoing Pokémon
- **THEN** the trading engine switches to Gen II without resetting the configured Pokémon data to a blank default species

### Requirement: Hardware Status RGB LED on GPIO 48
The system SHALL control the onboard WS2812 addressable RGB LED on GPIO 48 to visually display the current trade engine state using colors matching the web dashboard.

#### Scenario: Displaying waiting state
- **WHEN** the trade engine is waiting for a Game Boy connection (`STATUS_WAITING_GB`)
- **THEN** the RGB LED illuminates in red

#### Scenario: Displaying connected state
- **WHEN** the link handshake completes with a Game Boy (`STATUS_LINK_CONNECTED`)
- **THEN** the RGB LED illuminates in yellow/orange

#### Scenario: Displaying in-room trade table state
- **WHEN** the trade engine enters the Cable Club trade room (`STATUS_IN_TRADE_ROOM`, `STATUS_SELECTING_POKEMON`, or `STATUS_CONFIRMING_DEAL`)
- **THEN** the RGB LED illuminates in blue

#### Scenario: Displaying trade completion state
- **WHEN** a trade successfully completes (`STATUS_TRADE_SUCCESS`)
- **THEN** the RGB LED illuminates in green

### Requirement: Gen II Link Data Patch Offset Alignment
The system SHALL align the patch list generator with the 2-byte Trainer ID offset used by Generation II Game Boy games to prevent memory corruption when escape bytes (`0xFE`) occur in party data.

#### Scenario: Patch list generation in Gen II
- **WHEN** `plist_create()` processes party data for a Gen II trading session
- **THEN** the offset calculation incorporates the 2-byte Trainer ID prefix so that patch list indices correspond accurately to `wLinkPlayerPatchedData` in Game Boy WRAM
