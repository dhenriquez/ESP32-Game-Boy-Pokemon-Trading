#include "gb_link_driver.h"

GBLinkDriver GBLink;

GBLinkDriver::GBLinkDriver()
    : _clk_pin(4)
    , _so_pin(5)
    , _si_pin(6)
    , _in_byte(0)
    , _out_byte(0xFE)
    , _nobyte(0xFE)
    , _shift_count(0)
    , _last_bit_micros(0)
    , _running(false)
    , _last_transfer_micros(0)
    , _callback(NULL)
    , _cb_context(NULL)
{
}

void GBLinkDriver::begin(uint8_t clk_pin, uint8_t so_pin, uint8_t si_pin) {
    _clk_pin = clk_pin;
    _so_pin = so_pin;
    _si_pin = si_pin;

    pinMode(_clk_pin, INPUT_PULLUP);
    pinMode(_so_pin, INPUT_PULLUP);
    pinMode(_si_pin, OUTPUT);

    digitalWrite(_si_pin, HIGH); // Default high line
}

void GBLinkDriver::setCallback(gb_transfer_callback_t cb, void* context) {
    _callback = cb;
    _cb_context = context;
}

void GBLinkDriver::setNoDataByte(uint8_t no_data) {
    _nobyte = no_data;
}

void GBLinkDriver::setNextByte(uint8_t next_byte) {
    _out_byte = next_byte;
}

void GBLinkDriver::start() {
    if (_running) return;
    _shift_count = 0;
    _in_byte = 0;
    _out_byte = _nobyte;
    _running = true;

    attachInterruptArg(digitalPinToInterrupt(_clk_pin), isrHandler, this, CHANGE);
}

void GBLinkDriver::stop() {
    if (!_running) return;
    _running = false;
    detachInterrupt(digitalPinToInterrupt(_clk_pin));
    digitalWrite(_si_pin, HIGH);
}

bool GBLinkDriver::isConnected() {
    if (!_running) return false;
    return (micros() - _last_transfer_micros) < 500000; // Active within last 500ms
}

void IRAM_ATTR GBLinkDriver::isrHandler(void* arg) {
    GBLinkDriver* self = (GBLinkDriver*)arg;
    if (!self || !self->_running) return;

    bool clk_high = digitalRead(self->_clk_pin);

    if (!clk_high) {
        // Falling edge: set the next bit on SI for the Game Boy to sample
        digitalWrite(self->_si_pin, (self->_out_byte & 0x80) ? HIGH : LOW);
        self->_out_byte <<= 1;
    } else {
        // Rising edge: sample SO from Game Boy
        uint32_t now = micros();
        if ((now - self->_last_bit_micros) > 500) {
            self->_shift_count = 0;
        }
        self->_last_bit_micros = now;

        self->_in_byte = (self->_in_byte << 1) | (digitalRead(self->_so_pin) ? 1 : 0);
        self->_shift_count++;

        if (self->_shift_count >= 8) {
            self->_shift_count = 0;
            self->_last_transfer_micros = now;

            uint8_t next_out = self->_nobyte;
            if (self->_callback) {
                next_out = self->_callback(self->_cb_context, self->_in_byte);
            }
            self->_out_byte = next_out;
        }
    }
}
