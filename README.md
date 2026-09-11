# ESP32 Game Boy Pokémon Trading & Captive Portal 🎮⚡

Port autónomo del proyecto [Flipper-Zero-Game-Boy-Pokemon-Trading](https://github.com/kbembedded/Flipper-Zero-Game-Boy-Pokemon-Trading) de **KBEmbedded** a microcontroladores **ESP32** (específicamente optimizado para la placa compacta **ESP32-S3 SuperMini**).

Permite transferir, recibir y modificar Pokémon de **Generación I** (Red / Blue / Yellow) y **Generación II** (Gold / Silver / Crystal) a través de un cable Link físico de Game Boy, controlado de forma 100% inalámbrica desde cualquier teléfono móvil o PC mediante un **Portal Cautivo Wi-Fi autónomo** sin necesidad de conexión a Internet ni apps externas.

---

## 🌟 Características Principales

- **Doble Flujo de Intercambio**:
  - **⚡ Inyector / Salida**: Crea o selecciona Pokémon con ataques personalizables, nivel, mote, IVs/EVs máximos, shininess y objetos equipados (Gen II), además de presets directos (Gengar, Machamp, Alakazam, Golem, Steelix, Scizor, Mew, Celebi).
  - **📥 Receptor & Modificador**: Captura el Pokémon que envíe la Game Boy a la memoria del ESP32, visualiza su objeto equipado, ataques y estadísticas en la pestaña *"Recibido"*, permite modificarlo y dejarlo listo para devolvérselo a la consola.
- **Sincronización en Mesa de Trade (`in_table`)**:
  - Si configuras un Pokémon cuando la Game Boy ya está sentada en la mesa del Club del Cable, la interfaz te avisa para pulsar **B** y volver a interactuar con la mesa, asegurando que la Game Boy descargue el bloque de datos actualizado de 441 bytes.
- **Sprites Monocromáticos Embebidos (PROGMEM)**:
  - Los 252 sprites originales de Pokémon (creados por [ProteanReverie](https://github.com/ProteanReverie) para el proyecto de Flipper Zero) están compilados directamente en la memoria Flash del ESP32, funcionando a alta velocidad y de forma 100% offline.
- **Arquitectura Dual-Core FreeRTOS**:
  - **Core 0**: Wi-Fi SoftAP (`GB-PokeTrader`), DNS Cautivo (puerto 53), Servidor Web Asíncrono y WebSockets.
  - **Core 1**: Driver SPI Slave de ultra-alta velocidad por interrupciones GPIO dedicado exclusivamente al protocolo Game Boy Link (~8 kHz), inmune a fluctuaciones o jitter de la red Wi-Fi.
- **LED de Estado RGB Físico (GPIO 48)**:
  - El LED RGB WS2812 integrado en el ESP32-S3 SuperMini refleja en tiempo real los mismos colores que el panel web para diagnosticar la comunicación al instante sin mirar la pantalla.

---

## 🔌 Cableado, Identificación con Tester y Soldadura

> [!CAUTION]
> **¡NUNCA CONFÍES EN LOS COLORES DE LOS CABLES INTERNOS!**  
> Los cables Link de Game Boy de terceros (comprados en AliExpress, Amazon o tiendas retro) utilizan combinaciones de colores internas totalmente aleatorias según la fábrica o lote.  
> **Es obligatorio identificar cada cable con un multímetro (tester) en modo continuidad antes de soldar**.

### 1. Conector del Game Boy Link Cable

Mirando de frente el conector hembra o macho del cable Link:

```
       _____/-----\_____
      |  1     3     5  |
      |  2     4     6  |
      \_________________/
```

| Pin Conector GB | Nombre Señal | Conexión ESP32-S3 SuperMini | Función |
|:---:|:---:|:---:|:---|
| **Pin 1** | **VCC (+5V)** | *No conectar (cortar / aislar)* | Alimentación de la consola (el ESP32 se alimenta por USB-C) |
| **Pin 2** | **SO (Serial Out)** | **GPIO 6** *(SI del ESP32)* | Datos que salen de la GB y entran al ESP32 |
| **Pin 3** | **SI (Serial In)** | **GPIO 5** *(SO del ESP32)* | Datos que salen del ESP32 y entran a la GB |
| **Pin 4** | **SD (Sin uso)** | *No conectar (cortar / aislar)* | No utilizado en trades |
| **Pin 5** | **SCK (Clock)** | **GPIO 4** *(CLK del ESP32)* | Reloj maestro generado por la GB (~8 kHz) |
| **Pin 6** | **GND (Tierra)** | **GND** | **Tierra común obligatoria** entre la GB y el ESP32 |

---

### 2. Guía Paso a Paso con el Tester (Multímetro)

1. **Ajuste del Tester**: Pon el multímetro en la escala de **Continuidad** (icono de diodo / señal sonora de bip). Junta ambas puntas para confirmar que emite sonido.
2. **Pelado de la funda exterior**: Con cuidado de no cortar los hilos interiores, retira unos 2 a 3 cm de la funda plástica exterior del extremo del cable Link.
3. **Mapeo de Pines**:
   - Introduce una aguja fina o la punta fina del multímetro en el agujero del **Pin 6** del conector frontal.
   - Toca uno por uno los cables pelados con la otra punta hasta que el tester pite de forma continua (0 Ω).
   - **Anota el color encontrado para el Pin 6 (GND)**.
   - Repite el procedimiento con el **Pin 5 (SCK / Reloj)**, **Pin 2 (SO)** y **Pin 3 (SI)**.
4. **Verificación de Cortocircuitos (Muy Importante)**:
   - Prueba continuidad entre los 4 cables identificados entre sí.
   - **Ningún par de cables debe pitar entre sí**. Si pita alguno, hay hilos tocándose o el cable está dañado internamente.
5. **Cables sobrantes**: Los cables correspondientes al Pin 1 y Pin 4 córtalos o cúbrelos con cinta aislante / termorretráctil para evitar contactos accidentales.

---

### 3. Procedimiento de Soldadura

1. **Pre-estañado**: Aplica un punto pequeño de estaño y flux a las puntas peladas de los 4 cables y a los pads del ESP32-S3 SuperMini (**GND**, **GPIO 4**, **GPIO 5**, **GPIO 6**).
2. **Soldado**:
   - Cable del **Pin 6 (GND)** ➡️ Pad **GND**
   - Cable del **Pin 5 (SCK)** ➡️ Pad **GPIO 4**
   - Cable del **Pin 2 (SO)** ➡️ Pad **GPIO 6**
   - Cable del **Pin 3 (SI)** ➡️ Pad **GPIO 5**
3. **Inspección Final con Tester**:
   - Con el cable ya soldado, coloca una punta del multímetro en el conector frontal de la Game Boy y la otra directamente en el pin correspondiente de la placa ESP32. Debe pitar de inmediato.
   - Verifica que no haya puentes accidentales de estaño entre **GPIO 4**, **GPIO 5** y **GPIO 6**.

> [!NOTE]
> **Voltajes lógicos (5V vs 3.3V)**:  
> La Game Boy opera con niveles lógicos de 5V y el ESP32 a 3.3V. Las salidas de la Game Boy hacia los pines GPIO 4 y GPIO 6 son toleradas por las etapas de entrada del chip para pruebas habituales, pero para instalaciones permanentes se puede intercalar un divisor resistivo simple (ej. 1kΩ / 2kΩ) o un conversor de nivel lógico bidireccional de 3.3V/5V. La salida GPIO 5 (3.3V) es reconocida de forma nativa por la Game Boy como nivel alto (HIGH).

---

## 🚦 Diagnóstico Visual con LED RGB (GPIO 48)

El ESP32-S3 SuperMini cuenta con un LED direccionable WS2812 en el pin **GPIO 48**. El firmware lo utiliza para indicar el estado de la conexión en tiempo real:

| Color del LED | Estado del Motor | Significado / Acción |
|:---:|:---|:---|
| 🔴 **Rojo** | `STATUS_WAITING_GB` | ESP32 listo y Wi-Fi activo. Esperando que la Game Boy se encienda o genere señal de reloj. |
| 🟡 **Ámbar / Amarillo** | `STATUS_LINK_CONNECTED` | **¡Conexión detectada!** Handshake de 8 kHz sincronizado. *(Confirma que GND, SCK y SO están bien soldados)*. |
| 🔵 **Azul** | `STATUS_IN_TRADE_ROOM` | Ambos jugadores dentro del Club del Cable o sentados en la mesa de intercambio. |
| 🟣 **Púrpura / Magenta** | `STATUS_EXCHANGING_DATA` | Transfiriendo bloques de datos o reproduciendo la animación de intercambio. |
| 🟢 **Verde** | `STATUS_TRADE_SUCCESS` | **¡Intercambio completado con éxito!** Pokémon transferido correctamente. |

---

## 🚀 Compilación y Flasheo

El proyecto se compila y graba fácilmente usando **PlatformIO**:

### Desde la Terminal:
```bash
# Compilar firmware
pio run

# Flashear al ESP32 (por cable USB-C)
pio run -t upload

# Abrir monitor serial (115200 baudios)
pio device monitor
```

*(Si utilizas PlatformIO dentro del entorno virtual o VS Code, puedes usar directamente el botón de **Upload** de la barra inferior).*

---

## 📱 Guía de Uso Rápido

1. **Alimentación**: Conecta el ESP32-S3 mediante su puerto USB-C a una batería portátil (powerbank), cargador o PC. El LED RGB se iluminará en **rojo**.
2. **Conexión Wi-Fi**:
   - En tu smartphone o portátil, conéctate a la red Wi-Fi: **`GB-PokeTrader`** (sin contraseña).
   - Se abrirá la ventana de bienvenida / portal cautivo automáticamente.
   - Si no se abre sola, entra en tu navegador a: **`http://192.168.4.1`**.
3. **Configura tu Pokémon**:
   - Ve a la pestaña **⚡ Enviar**, selecciona especie, nivel, ataques, mote o pulsa un preset (ej. Gengar, Mew, Celebi).
   - En Gen II puedes elegir si es **Shiny** y qué **Objeto Equipado** llevará.
   - Pulsa **💾 Cargar en la Mesa de Intercambio**.
4. **En tu Game Boy**:
   - Conecta el cable Link a la consola.
   - Enciende el juego (R/B/Y o G/S/C) y entra al **Centro Pokémon**.
   - Habla con la recepcionista del **Club del Cable** (Coliseo / Sala de Intercambio).
   - Guarda la partida cuando lo solicite y entra a la sala.
   - Al sentarte en la mesa, el intercambio comenzará automáticamente.
5. **Recepción y Modificación**:
   - Cuando termine el intercambio, el Pokémon que envió la Game Boy se mostrará en la pestaña **📥 Recibido** con su nivel, ataques y objeto.
   - Puedes cambiar sus estadísticas o ataques y pulsar **📤 Preparar para Devolver a Game Boy** para reenviárselo modificado.

---

## 👏 Créditos y Autoría

- **Desarrollo y Adaptación a ESP32**: **Daniel Henríquez Sandoval** ([@dhenriquez](https://github.com/dhenriquez)) — Arquitectura Dual-Core FreeRTOS, interfaz web de portal cautivo autónomo, sincronización en mesa de trade, integración de learnsets por nivel, soporte de objetos equipados Gen II y control de LED RGB de estado.
- **Proyecto Original Flipper Zero**: [kbembedded/Flipper-Zero-Game-Boy-Pokemon-Trading](https://github.com/kbembedded/Flipper-Zero-Game-Boy-Pokemon-Trading) creado por **KBEmbedded**.
- **Sprites Monocromáticos**: Diseñados y adaptados por [ProteanReverie](https://github.com/ProteanReverie).
- **Ingeniería Inversa del Cable Link de Pokémon**:
  - [Disassembly de Pokémon Yellow](https://github.com/pret/pokeyellow)
  - [Disassembly de Pokémon Gold/Silver](https://github.com/pret/pokegold)
  - [Arduino Spoofing Game Boy Pokemon Trades](https://github.com/EstebanFuentealba/Arduino-Spoofing-Gameboy-Pokemon-Trades) de Esteban Fuentealba.

---

<p align="center">
  Desarrollado con ❤️ y nostalgia retro por <b>Daniel Henríquez Sandoval</b> desde Chile 🇨🇱
</p>