## Why

Actualmente, cuando el ESP32-S3 Supermini completa un intercambio con la Game Boy, los datos del Pokémon recibido residen únicamente en la memoria RAM volátil. Si el dispositivo se reinicia o se realiza otro trade, la información se pierde. Además, los entrenadores necesitan un registro histórico persistente ("Bill's PC"), la capacidad de conservar todos los detalles originales de procedencia (nivel, mote, entrenador emisor, OT original, OT ID, ataques, ítem, IVs/EVs), editar dichos Pokémon para ajustarlos o devolverlos a la Game Boy respetando las reglas de la Cápsula del Tiempo entre Gen I y Gen II, y actualizar los valores por defecto del sistema al entrenador del usuario (`DHNRQZ` / `20487`).

## What Changes

- **Almacenamiento Persistente en LittleFS**: Implementación de guardado automático en `/bills_pc.json` cada vez que ocurre un intercambio exitoso (`STATUS_TRADE_SUCCESS`), reteniendo todos los datos de origen (nivel, especie, nombre, mote, trainer name, OT name, OT ID, movimientos, ítem, IVs, EVs, timestamps y generación de origen).
- **Interfaz Web "Bill's PC"**: Nueva pestaña en la UI del portal cautivo que permite visualizar los Pokémon almacenados en forma de tarjetas con sus sprites, detalles de procedencia y estadísticas.
- **Modificación y Carga de Pokémon**:
  - Modal o interfaz de edición para modificar ataques, nivel, mote, ítem y estado shiny de cualquier Pokémon en el PC.
  - Botón "Cargar para Trade" para preparar directamente el Pokémon modificado como oferta de intercambio saliente.
- **Validación de Cápsula del Tiempo (Time Capsule Compatibility)**: Advertencia interactiva al modificar un Pokémon de Gen I con movimientos o ítems exclusivos de Gen II, marcándolo automáticamente como `target_gen = 2` para evitar desincronizaciones con cartuchos R/B/Y.
- **Backup e Importación**: Endpoints REST y botones en la UI para descargar (`export`) el archivo JSON completo del PC o importar/restaurar registros.
- **Ajuste de Identidad por Defecto**: Actualización del nombre de entrenador por defecto a `DHNRQZ` y del OT ID a `20487` en todo el sistema (en lugar de `ESP32` / `42069`).

## Capabilities

### New Capabilities
- `bills-pc-storage`: Gestión y persistencia en LittleFS del historial de Pokémon recibidos por trade en formato JSON, endpoints REST para listar, editar, eliminar, exportar e importar registros.
- `time-capsule-rules`: Validación y advertencias visuales de compatibilidad entre Gen I y Gen II al editar Pokémon o prepararlos para devolución hacia la Game Boy.

### Modified Capabilities
- `trainer-identity`: Actualización de los valores por defecto del Original Trainer (`DHNRQZ`) y Trainer ID (`20487`) en la inicialización y payloads del sistema.

## Impact

- **Almacenamiento / Flash**: Uso de LittleFS en partición de flash SPIFFS/LittleFS del ESP32-S3.
- **Código C++ / ESP32**:
  - Nuevos módulos o controladores para gestión de LittleFS y JSON (`ArduinoJson`).
  - [gb_trade_engine.cpp](file:///Users/danielhenriquezsandoval/Documents/Proyectos/ESP32-Game-Boy-Pokemon-Trading/src/gb_trade_engine.cpp), [web_server.cpp](file:///Users/danielhenriquezsandoval/Documents/Proyectos/ESP32-Game-Boy-Pokemon-Trading/src/web_server.cpp), [pokemon_data.cpp](file:///Users/danielhenriquezsandoval/Documents/Proyectos/ESP32-Game-Boy-Pokemon-Trading/src/pokemon_data.cpp).
- **Frontend Web**:
  - Pestaña "Bill's PC", visualizador de fichas, advertencia de Time Capsule y llamadas API correspondientes.
- **Dependencias**:
  - Requiere inicialización de LittleFS en `main.cpp`. Utiliza `ArduinoJson` (ya disponible en `platformio.ini`).
