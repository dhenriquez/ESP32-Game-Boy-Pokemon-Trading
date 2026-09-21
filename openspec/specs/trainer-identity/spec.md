# trainer-identity Specification

## Purpose
Configuración de los valores predeterminados de identidad del entrenador (Original Trainer) en el firmware y la interfaz gráfica.

## Requirements

### Requirement: Valores por Defecto de Entrenador
El sistema SHALL utilizar como valores por defecto en todo el firmware el nombre de entrenador `DHNRQZ` y el ID de entrenador `20487`.

#### Scenario: Inicialización de bloque de trade
- **WHEN** el sistema inicializa una estructura de Pokémon o datos de intercambio por defecto
- **THEN** el nombre del entrenador (Trainer Name) y del OT (Original Trainer) se establecerán en `DHNRQZ`, y el OT ID se fijará en `20487`.

#### Scenario: Carga de Pokémon saliente sin OT especificado
- **WHEN** la interfaz web o el motor de trade configura un nuevo Pokémon sin especificar un OT personalizado
- **THEN** se aplicará automáticamente el nombre `DHNRQZ` y el ID `20487`.
