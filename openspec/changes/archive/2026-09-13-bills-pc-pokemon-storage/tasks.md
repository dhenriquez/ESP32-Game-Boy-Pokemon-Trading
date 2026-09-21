## 1. Actualización de Valores por Defecto de Entrenador

- [x] 1.1 Actualizar el nombre de entrenador por defecto a `DHNRQZ` y OT ID a `20487` en `pokemon_data.cpp`
- [x] 1.2 Actualizar el OT ID y Trainer Name por defecto en `gb_trade_engine.cpp`
- [x] 1.3 Actualizar los valores por defecto en los endpoints de `web_server.cpp`

## 2. Almacenamiento Persistente en LittleFS (Bill's PC Storage)

- [x] 2.1 Inicializar LittleFS en `main.cpp`
- [x] 2.2 Crear módulo/servicio `bills_pc_storage` para gestionar `/bills_pc.json` (lectura, guardado atómico, inserción, actualización, eliminación)
- [x] 2.3 Integrar el auto-guardado en el callback de evento `STATUS_TRADE_SUCCESS` en `main.cpp` / `gb_trade_engine.cpp` capturando todos los datos de origen (nivel, mote, trainer name, OT name, OT ID, ataques, ítem, IVs/EVs, timestamps y `origin_gen`)

## 3. Endpoints REST en WebServer

- [x] 3.1 Implementar GET `/api/pc` para obtener la lista de Pokémon en Bill's PC
- [x] 3.2 Implementar PUT `/api/pc/:id` para actualizar datos modificados de un Pokémon guardado
- [x] 3.3 Implementar DELETE `/api/pc/:id` para remover un Pokémon del PC
- [x] 3.4 Implementar POST `/api/pc/:id/load` para preparar un Pokémon del PC como oferta saliente en `TradeEngine`
- [x] 3.5 Implementar GET `/api/pc/export` y POST `/api/pc/import` para respaldar o restaurar la colección JSON

## 4. Interfaz Web (Pestaña Bill's PC y Validación Time Capsule)

- [x] 4.1 Añadir pestaña "Bill's PC" en la barra de navegación del frontend web (`web_assets.h`)
- [x] 4.2 Diseñar las tarjetas de Pokémon en Bill's PC mostrando sprites, estadísticas, motes y origen
- [x] 4.3 Crear modal de edición de Pokémon en Bill's PC (ataques, nivel, mote, shiny, ítem)
- [x] 4.4 Añadir validación y advertencia visual de Cápsula del Tiempo si un Pokémon de Gen I recibe ítems o ataques de Gen II (IDs > 165), marcando `target_gen = 2`
- [x] 4.5 Agregar botones de "Cargar para Trade", "Eliminar", "Exportar JSON" e "Importar JSON"

## 5. Verificación y Compilación

- [x] 5.1 Compilar con PlatformIO (`pio run -e esp32-s3-supermini`) para comprobar ausencia de errores sintácticos y tamaño de flash
- [x] 5.2 Validar la integridad de los endpoints REST y la consistencia de datos de salida
