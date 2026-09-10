#pragma once

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t (*gb_transfer_callback_t)(void* context, uint8_t in_byte);

class GBLinkDriver {
public:
    GBLinkDriver();
    void begin(uint8_t clk_pin, uint8_t so_pin, uint8_t si_pin);
    void setCallback(gb_transfer_callback_t cb, void* context);
    void setNoDataByte(uint8_t no_data);
    void setNextByte(uint8_t next_byte);
    void start();
    void stop();
    bool isConnected();
    uint32_t getLastActivityTime() const { return _last_transfer_micros; }

    static void IRAM_ATTR isrHandler(void* arg);

private:
    uint8_t _clk_pin;
    uint8_t _so_pin;
    uint8_t _si_pin;

    volatile uint8_t _in_byte;
    volatile uint8_t _out_byte;
    volatile uint8_t _nobyte;
    volatile uint8_t _shift_count;
    volatile uint32_t _last_bit_micros;
    volatile bool _running;
    volatile uint32_t _last_transfer_micros;

    gb_transfer_callback_t _callback;
    void* _cb_context;
};

extern GBLinkDriver GBLink;
