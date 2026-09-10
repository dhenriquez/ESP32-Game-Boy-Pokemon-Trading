# ESP32 Game Boy Pokémon Trading & Captive Portal 🎮⚡

Port directo del proyecto [Flipper-Zero-Game-Boy-Pokemon-Trading](file:///c:/Users/dhenriquez/Documents/Sitios/ESP32-Game-Boy-Pokemon-Trading/Flipper-Zero-Game-Boy-Pokemon-Trading) a microcontroladores **ESP32** (específicamente optimizado para **ESP32-S3 SuperMini**).

Permite inyectar, recibir y modificar Pokémon de **Generación I** (Red / Blue / Yellow) y **Generación II** (Gold / Silver / Crystal) a través de un cable Link físico de Game Boy, controlado de forma 100% inalámbrica desde un teléfono o PC mediante un **Portal Cautivo Wi-Fi autónomo** sin necesidad de conexión a Internet.

---

## 🌟 Características Principales

- **Doble Flujo de Intercambio**:
  - **Inyector**: Crea o selecciona Pokémon preconfigurados (evoluciones por intercambio como Gengar, Machamp, Alakazam, Golem, Steelix, Scizor, o legendarios/eventos como Mew y Celebi).
  - **Receptor & Modificador**: Recibe el Pokémon que envíe la Game Boy, muestra sus estadísticas, ataques y mote en la web, permite modificarlo y volver a transferirlo.
- **Sprites Originales Autónomos**:
  - Utiliza los sprites monocromáticos auténticos de 56x56 px ubicados en [`Flipper-Zero-Game-Boy-Pokemon-Trading/sprites/`](file:///c:/Users/dhenriquez/Documents/Sitios/ESP32-Game-Boy-Pokemon-Trading/Flipper-Zero-Game-Boy-Pokemon-Trading/sprites/).
  - Los 252 sprites están compilados en la memoria flash interna del ESP32 (**PROGMEM**), garantizando que la interfaz web funcione offline dentro del portal cautivo.
- **Arquitectura Dual-Core FreeRTOS**:
  - **Core 0**: Wi-Fi SoftAP, DNS Cautivo (puerto 53), Servidor Web Asíncrono y WebSockets.
  - **Core 1**: Driver SPI Slave de ultra-alta velocidad por interrupciones GPIO dedicado al protocolo Game Boy Link (~8 kHz), inmune al jitter de red.
- **Vista Previa en Navegador**:
  - Carpeta [`html/`](file:///c:/Users/dhenriquez/Documents/Sitios/ESP32-Game-Boy-Pokemon-Trading/html/) con simulador interactivo de cable link para probar la interfaz en tu PC antes de conectar la consola.

---

## 🎨 Gestión y Sincronización de Sprites

Los sprites provienen directamente de la carpeta de Flipper Zero. Para sincronizarlos o regenerar los arrays C++ embebidos en el ESP32, se incluye un script automatizado:

```bash
python scripts/generate_sprites.py
```

Este script:
1. Lee los 252 archivos PNG de [`Flipper-Zero-Game-Boy-Pokemon-Trading/sprites/`](file:///c:/Users/dhenriquez/Documents/Sitios/ESP32-Game-Boy-Pokemon-Trading/Flipper-Zero-Game-Boy-Pokemon-Trading/sprites/).
2. Los copia a la carpeta de vista previa [`html/sprites/`](file:///c:/Users/dhenriquez/Documents/Sitios/ESP32-Game-Boy-Pokemon-Trading/html/sprites/).
3. Genera [`src/pokemon_sprites.cpp`](file:///c:/Users/dhenriquez/Documents/Sitios/ESP32-Game-Boy-Pokemon-Trading/src/pokemon_sprites.cpp) y [`include/pokemon_sprites.h`](file:///c:/Users/dhenriquez/Documents/Sitios/ESP32-Game-Boy-Pokemon-Trading/include/pokemon_sprites.h) en formato binario `PROGMEM` (~130 KB).

---

## 🔌 Cableado y Diagrama de Pines

El conector del Game Boy Link Cable (vista frontal del conector de la consola / cable):

```
       _____/-----\_____
      |  1     3     5  |
      |  2     4     6  |
      \_________________/
```

| Pin Cable GB | Señal Game Boy | Pin ESP32-S3 SuperMini | Descripción |
|:---:|:---:|:---:|:---|
| **1** | **VCC (+5V)** | *No conectar* | Alimentación de la GB (opcional / usar USB del ESP32) |
| **2** | **SO (Serial Out)** | **GPIO 6 (SI)** | Datos que salen de la GB y entran al ESP32 |
| **3** | **SI (Serial In)** | **GPIO 5 (SO)** | Datos que salen del ESP32 y entran a la GB |
| **4** | *(SD / Sin uso)* | *No conectar* | No utilizado en trades |
| **5** | **SCK (Clock)** | **GPIO 4 (CLK)** | Reloj maestro generado por la Game Boy (~8 kHz) |
| **6** | **GND (Tierra)** | **GND** | **Común Obligatorio** entre Game Boy y ESP32 |

> [!WARNING]
> **Protección de Voltaje (5V a 3.3V)**:
> La Game Boy (DMG y Color) opera con lógica de **5V**, mientras que el ESP32 trabaja a **3.3V**.
> - En las líneas que entran al ESP32 (**SCK/CLK** y **SO de GB hacia GPIO 6**), se recomienda utilizar un divisor resistivo (ej. 1kΩ y 2kΩ) o un convertidor de nivel lógico (Level Shifter bidireccional).
> - La línea de salida del ESP32 (**GPIO 5 hacia SI de GB**) típicamente es reconocida por la Game Boy a 3.3V sin problemas.

---

## 🚀 Compilación y Flasheo

El proyecto está configurado con **PlatformIO**:

### 1. Desde la Terminal (PlatformIO Core CLI)
```bash
# Compilar el proyecto
python -m platformio run

# Flashear al ESP32 (conectado por USB)
python -m platformio run --target upload

# Monitor Serie (115200 baud)
python -m platformio device monitor
```

### 2. Desde Visual Studio Code
1. Abre la carpeta del proyecto en VS Code.
2. Asegúrate de tener instalada la extensión oficial **PlatformIO IDE**.
3. Haz clic en el icono de **Checkmark (Build)** en la barra inferior para compilar.
4. Haz clic en la **Flecha (Upload)** para subir el firmware a tu ESP32.

---

## 📱 Modo de Uso

1. **Alimentación**: Conecta el ESP32 por cable USB a un cargador o batería portátil.
2. **Conexión Wi-Fi**:
   - En tu teléfono o PC, busca la red Wi-Fi: **`GB-PokeTrader`** (sin contraseña).
   - Conéctate. La pantalla de inicio de sesión o portal cautivo se abrirá automáticamente.
   - Si no abre de inmediato, entra en tu navegador a: **`http://192.168.4.1`**
3. **Configura tu Pokémon**:
   - En la pestaña **⚡ Enviar**, elige una especie, nivel, ataques, mote o pulsa un preset instantáneo.
   - Haz clic en **💾 Cargar en la Mesa de Intercambio**.
4. **En tu Game Boy**:
   - Conecta el cable Link al ESP32.
   - Entra al Centro Pokémon y habla con la recepcionista del **Club del Cable**.
   - Ingresa a la sala de intercambio. En cuanto ambos personajes se sienten a la mesa, el intercambio comenzará automáticamente.
5. **Recepción y Modificación**:
   - El Pokémon recibido de la consola aparecerá en la pestaña **🔄 Recibido** con su sprite, ataques y estadísticas.
   - Puedes ajustar sus parámetros y enviarlo de vuelta en un segundo intercambio.