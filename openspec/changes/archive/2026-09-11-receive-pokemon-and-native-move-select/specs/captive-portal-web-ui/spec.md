## MODIFIED Requirements

### Requirement: Pokémon Configuration and Customization
The system SHALL provide form controls to configure all trade-relevant Pokémon data fields including species, level, nickname, OT name, OT ID, moves, IV/EV presets, and held items (Gen II).

#### Scenario: Configuring individual IV and EV presets
- **WHEN** a preset (such as Max IV/EV or Competitive) is selected
- **THEN** the internal structure reflects the exact byte layout required by the game generation

#### Scenario: Selecting Gen II held item
- **WHEN** the user enables Gen II options and chooses an item from the held items dropdown
- **THEN** the selected item is mapped to its internal ID and sent in the trade block payload

#### Scenario: Native dropdown move selection
- **WHEN** a user selects an attack from any of the four move slots (`m0` through `m3`)
- **THEN** the browser displays a standard native `<select>` dropdown populated with move names and IDs, working reliably across mobile browsers and captive portals

#### Scenario: Filtering attacks by active generation
- **WHEN** the user switches between Gen I and Gen II
- **THEN** the move select elements filter options to show only legal Gen I moves when Gen I is active, and all 251 moves when Gen II is active

#### Scenario: Applying quick presets
- **WHEN** a user clicks a preset (such as trade evolution or legendary)
- **THEN** the form automatically populates with valid legal attributes for the chosen Pokémon, synchronizing the species dropdown, nickname, item selector, and selecting attack options in the move dropdowns

### Requirement: Received Pokémon Inspection and Modification
The system SHALL display the party data received from the Game Boy and allow the user to modify its attributes before trading it back.

#### Scenario: Displaying received Pokemon
- **WHEN** the Game Boy completes a trade with the system
- **THEN** the web interface automatically switches to the Received tab and displays the received Pokémon's current level, stats, moves selected in native dropdowns, shiny status, and held item

#### Scenario: Modifying and queueing back
- **WHEN** the user modifies attributes (e.g., changes level, sets shininess, or alters moves) and clicks "Preparar para devolver"
- **THEN** the updated data is re-encoded into the trade block buffer ready for the next trade exchange
