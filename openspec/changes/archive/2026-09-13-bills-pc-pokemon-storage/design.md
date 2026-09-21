## Context

El proyecto actual permite comunicar un Game Boy con un microcontrolador ESP32-S3 Supermini simulando un segundo Game Boy mediante el cable Link. Cuando se completa un intercambio, el Pokémon recibido es procesado en RAM por `TradeEngine` y transmitido por WebSockets mediante `WebManager.broadcastReceivedPokemon()`. Sin embargo, no existe persistencia en memoria no volátil; al apagar el ESP32 o realizar un nuevo trade, se sobrescribe la memoria.

Se requiere implementar un sistema de almacenamiento y gestión persistente llamado **"Bill's PC"**, que guarde los Pokémon recibidos en un archivo JSON en LittleFS (`/bills_pc.json`), permita visualizarlos y modificarlos en una pestaña web dedicada, respete las reglas de compatibilidad de la Cápsula del Tiempo (Time Capsule) entre Gen I y Gen II, y actualice los valores por defecto del sistema al entrenador `DHNRQZ` y OT ID `20487`.

## Goals / Non-Goals

**Goals:**
- Almacenar persistentemente en LittleFS cada Pokémon recibido exitosamente durante un intercambio (`STATUS_TRADE_SUCCESS`), reteniendo todos los datos de origen (nivel, especie, nombre, mote, trainer name, OT name, OT ID, movimientos, ítem, IVs, EVs, timestamps y generación de origen).
- Proveer una API REST para listar, editar, eliminar, exportar e importar la colección de Bill's PC.
- Añadir la pestaña "Bill's PC" en la interfaz web para explorar los Pokémon almacenados como tarjetas interactivas.
- Permitir cargar cualquier Pokémon guardado hacia la mesa de trade (`outgoing`) para devolverlo o intercambiarlo.
- Implementar un editor para modificar parámetros del Pokémon (mote, movimientos, nivel, ítem, shiny).
- Detectar y alertar cuando un Pokémon originario de Gen I adquiere características exclusivas de Gen II (ítem o movimientos con ID > 165), marcándolo con `target_gen = 2` y notificando al usuario.
- Actualizar el nombre de entrenador por defecto a `DHNRQZ` y el ID a `20487`.

**Non-Goals:**
- Simular la lógica de batalla o cálculo de experiencia avanzada fuera de lo ya soportado en `pokemon_data.cpp`.
- Almacenar partidas completas de Game Boy (saves .sav); solo se almacenan entidades individuales de Pokémon transferidos.
- Soporte para Generación III o superior (GBA).

## Decisions

### 1. Sistema de Archivos: LittleFS sobre NVS/EEPROM
- **Decisión**: Utilizar LittleFS con un archivo JSON estructurado (`/bills_pc.json`).
- **Alternativas consideradas**:
  - *NVS (Non-Volatile Storage)*: Demasiado restrictivo para blobs dinámicos o arrays grandes; engorroso para exportar/importar.
  - *Estructura binaria fija*: Eficiente en espacio, pero poco legible y difícil de inspeccionar o exportar directamente hacia la interfaz web.
- **Razón**: LittleFS en ESP32 es resistente a cortes de energía repentinos y `ArduinoJson` maneja la serialización y deserialización de forma robusta con bajo overhead.

### 2. Estructura de Datos de Bill's PC
- **Decisión**: Cada entrada guardará:
  ```json
  {
    "id": "pkmn_1694612345",
    "species": 6,
    "name": "CHARIZARD",
    "nickname": "CHARIZARD",
    "level": 50,
    "origin_gen": 1,
    "target_gen": 1,
    "trainer_name": "RED",
    "ot_name": "RED",
    "ot_id": 12345,
    "item": 0,
    "shiny": false,
    "moves": [82, 53, 7, 126],
    "ivs": { "hp": 8, "atk": 9, "def": 8, "spd": 8, "spc": 8 },
    "evs": { "hp": 2000, "atk": 3000, "def": 2500, "spd": 3500, "spc": 3000 },
    "created_at": 1694612345
  }
  ```
- **Razón**: Contiene todo lo necesario para reconstruir fielmente tanto la visualización web como los buffers de intercambio para `TradeEngine.configureOutgoingPokemon()`.

### 3. Validación de Cápsula del Tiempo (Time Capsule)
- **Decisión**: Validación en cliente y servidor. Si `origin_gen == 1` pero el usuario equipa un `item > 0` o un movimiento con `id > 165` (rango de movimientos añadidos en Gen II):
  - El sistema actualiza `target_gen = 2`.
  - La interfaz Web emite una advertencia clara: *"Este Pokémon contiene elementos exclusivos de Gen II. No podrá transferirse a cartuchos de Gen I."*
- **Razón**: Evita corrupción de datos o congelamiento en el juego de Gen I si el usuario intenta enviar un Pokémon incompatible.

### 4. Valores de Entrenador por Defecto
- **Decisión**: Modificar las constantes y defaults en `pokemon_data.cpp`, `gb_trade_engine.cpp` y `web_server.cpp`:
  - Default OT/Trainer Name: `DHNRQZ`
  - Default OT ID: `20487`
- **Razón**: `DHNRQZ` tiene 6 caracteres, respetando el límite estricto de 7 caracteres de la Game Boy en Gen I/II, y `20487` es un entero de 16 bits válido (`< 65536`).

## Risks / Trade-offs

- **[Riesgo: Límite de Flash o Fragmentación con LittleFS]** → *Mitigación*: Implementar un límite máximo configurable (ej. 100 Pokémon en PC) y escrituras atómicas (escribir en archivo temporal y renombrar a `/bills_pc.json`).
- **[Riesgo: Múltiples clientes web editando simultáneamente]** → *Mitigación*: Operaciones sincronizadas y persistencia inmediata tras cada petición POST/PUT/DELETE.
- **[Riesgo: Movimientos inválidos al transferir]** → *Mitigación*: Verificación del rango de IDs de movimientos antes de configurar el bloque de salida en el Trade Engine.
