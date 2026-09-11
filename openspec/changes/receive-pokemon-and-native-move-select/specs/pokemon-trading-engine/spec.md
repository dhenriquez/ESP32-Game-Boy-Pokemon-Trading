## MODIFIED Requirements

### Requirement: Bi-directional Trade Data Handling
The system SHALL support both transmitting configured Pokémon to the Game Boy and capturing received Pokémon transmitted from the Game Boy into memory.

#### Scenario: Injecting configured Pokemon
- **WHEN** an outgoing trade is confirmed on both devices
- **THEN** the Game Boy receives the configured party data slot 1 from the system

#### Scenario: Receiving and storing traded Pokemon
- **WHEN** the Game Boy completes a trade with the system
- **THEN** the system moves the incoming Pokémon at the selected index to slot 0 of the received buffer and adopts it into the outgoing buffer with its exact moves, level, trainer details, and attributes
