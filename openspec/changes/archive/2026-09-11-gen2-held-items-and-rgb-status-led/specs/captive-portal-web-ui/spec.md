## ADDED Requirements

### Requirement: Received Pokémon Held Item Inspection and Return
The system SHALL display the held item equipped on any Pokémon received from the Game Boy and allow the user to modify or preserve this held item when returning the Pokémon to the Game Boy.

#### Scenario: Displaying received held item
- **WHEN** the Game Boy transfers party data in Gen II and the Pokémon is holding an item
- **THEN** the web interface displays the item name on the Received tab summary and pre-selects the item in the return form's item selector

#### Scenario: Returning Pokémon with custom or preserved held item
- **WHEN** the user selects an item (or keeps the received item) in the Received tab and clicks "Preparar para Devolver a Game Boy"
- **THEN** the web application sends the selected item ID in the `/api/return_traded` payload and the trading engine stores it in the outgoing trade block buffer

### Requirement: Live Trade Table Synchronization Guidance
The system SHALL detect when an outgoing Pokémon is updated while the Game Boy is actively seated at the Cable Club trade table and notify the user to re-seat the Game Boy at the table.

#### Scenario: Configuring outgoing Pokémon while seated at trade table
- **WHEN** the user submits the Pokémon configuration form while the Game Boy link state is in the trade room table phase (`STATE_TRADE_SELECT` or `STATE_TRADE_PENDING`)
- **THEN** the web interface displays a clear alert or toast instructing the user to cancel (button B) on the Game Boy and interact with the table again so the Game Boy re-fetches the updated trade block
