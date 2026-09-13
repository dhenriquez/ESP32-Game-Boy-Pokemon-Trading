## ADDED Requirements

### Requirement: Almacenamiento persistente en LittleFS
El sistema SHALL almacenar en LittleFS (`/bills_pc.json`) cada Pokémon recibido exitosamente en un intercambio con la Game Boy (`STATUS_TRADE_SUCCESS`), persistiendo los datos de origen de forma no volátil.

#### Scenario: Guardado automático tras trade exitoso
- **WHEN** se completa un intercambio con la Game Boy y se emite `STATUS_TRADE_SUCCESS`
- **THEN** el sistema extrae los datos de `_received_pdata` y los guarda en `/bills_pc.json` conservando nivel, especie, nombre, mote, trainer name, OT name, OT ID, movimientos, ítem, IVs, EVs y generación de origen (`origin_gen`).

### Requirement: API REST de Bill's PC
El servidor web SHALL proveer endpoints HTTP para consultar, actualizar, eliminar, exportar e importar los Pokémon almacenados en Bill's PC.

#### Scenario: Listar Pokémon en Bill's PC
- **WHEN** un cliente envía una petición GET a `/api/pc`
- **THEN** el sistema responde con un array JSON conteniendo la lista completa de Pokémon almacenados con todos sus atributos.

#### Scenario: Modificar Pokémon en Bill's PC
- **WHEN** un cliente envía una petición PUT a `/api/pc/{id}` con nuevos datos (mote, nivel, movimientos, ítem, shiny)
- **THEN** el sistema actualiza el registro correspondiente en `/bills_pc.json` y retorna confirmación de éxito.

#### Scenario: Eliminar Pokémon de Bill's PC
- **WHEN** un cliente envía una petición DELETE a `/api/pc/{id}`
- **THEN** el sistema remueve dicho Pokémon de `/bills_pc.json` y devuelve código 200.

#### Scenario: Cargar Pokémon de Bill's PC a la mesa de trade
- **WHEN** un cliente envía una petición POST a `/api/pc/{id}/load`
- **THEN** el sistema configura los datos de ese Pokémon en `TradeEngine.configureOutgoingPokemon(...)` para que esté listo como oferta en el siguiente trade.

#### Scenario: Exportar e importar colección de Bill's PC
- **WHEN** un cliente accede a `/api/pc/export` mediante GET
- **THEN** el servidor envía el archivo `/bills_pc.json` como descarga JSON descargable.

### Requirement: Pestaña Web Bill's PC
La interfaz web SHALL incluir una pestaña dedicada "Bill's PC" donde se listan los Pokémon como tarjetas visuales con sus sprites oficiales y opciones de acción.

#### Scenario: Visualización de tarjeta de Pokémon
- **WHEN** el usuario hace clic en la pestaña "Bill's PC"
- **THEN** la interfaz muestra las tarjetas de cada Pokémon con su sprite, mote, nivel, generación de origen, OT y botones para "Cargar para Trade", "Modificar" y "Eliminar".
