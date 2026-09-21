## ADDED Requirements

### Requirement: Detección y Alerta de Compatibilidad de Cápsula del Tiempo
El sistema SHALL validar si un Pokémon originario de Generación I (`origin_gen == 1`) adquiere movimientos o ítems exclusivos de Generación II al ser modificado, impidiendo desincronizaciones con cartuchos Gen I.

#### Scenario: Asignación de ítem a Pokémon de Gen I
- **WHEN** un usuario equipa un ítem a un Pokémon con `origin_gen == 1`
- **THEN** la interfaz web SHALL mostrar una advertencia informando que no podrá ser devuelto a cartuchos de Gen I y marcará el campo `target_gen` como 2.

#### Scenario: Aprendizaje de movimiento exclusivo de Gen II
- **WHEN** un usuario selecciona un movimiento con ID superior a 165 para un Pokémon con `origin_gen == 1`
- **THEN** el sistema notificará que dicho ataque pertenece a Gen II y asignará `target_gen = 2`.

#### Scenario: Intento de cargar a trade en modo Gen I con contenido incompatible
- **WHEN** el usuario intenta enviar al trade un Pokémon marcado con `target_gen == 2` mientras el TradeEngine está configurado en Generación I
- **THEN** el sistema advertirá al usuario que debe cambiar a Generación II para transferir dicho Pokémon.
