#pragma once

#include <Arduino.h>

/**
 * Default Pin Definitions for ESP32-S3 SuperMini
 * 
 * Game Boy Link Cable Pinout (Looking into the Game Boy Link Port / Cable):
 *  Pin 1: VDD (+5V / +3.3V) -> DO NOT CONNECT directly if ESP32 is powered via USB-C!
 *  Pin 2: SO (Serial Out from Game Boy) -> Connect to ESP32 PIN_GB_SO (via voltage divider / level shifter)
 *  Pin 3: SI (Serial In to Game Boy)    -> Connect to ESP32 PIN_GB_SI (3.3V direct)
 *  Pin 4: SD (Serial Data)              -> Not connected (Gen 1 / Gen 2 trade does not use this)
 *  Pin 5: SC (Serial Clock from GB)     -> Connect to ESP32 PIN_GB_CLK (via voltage divider / level shifter)
 *  Pin 6: GND (Ground)                  -> Connect to ESP32 GND (Mandatory common ground)
 */

#if defined(BOARD_ESP32S3_SUPERMINI)
    #ifndef PIN_GB_CLK
        #define PIN_GB_CLK 4
    #endif
    #ifndef PIN_GB_SO
        #define PIN_GB_SO 5
    #endif
    #ifndef PIN_GB_SI
        #define PIN_GB_SI 6
    #endif
    #ifndef PIN_STATUS_LED
        #define PIN_STATUS_LED 48
    #endif
#elif defined(BOARD_ESP32_GENERIC)
    #ifndef PIN_GB_CLK
        #define PIN_GB_CLK 18
    #endif
    #ifndef PIN_GB_SO
        #define PIN_GB_SO 19
    #endif
    #ifndef PIN_GB_SI
        #define PIN_GB_SI 23
    #endif
    #ifndef PIN_STATUS_LED
        #define PIN_STATUS_LED 2
    #endif
#else
    #ifndef PIN_GB_CLK
        #define PIN_GB_CLK 4
    #endif
    #ifndef PIN_GB_SO
        #define PIN_GB_SO 5
    #endif
    #ifndef PIN_GB_SI
        #define PIN_GB_SI 6
    #endif
    #ifndef PIN_STATUS_LED
        #define PIN_STATUS_LED 48
    #endif
#endif
