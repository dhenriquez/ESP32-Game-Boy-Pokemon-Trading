#pragma once

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>

#include "pokemon_data.h"
#include "patch_list.h"
#include "gb_link_driver.h"

enum GBTradeState {
    STATE_NOT_CONNECTED,
    STATE_CONNECTED,
    STATE_TRADE_INIT,
    STATE_TRADE_RANDOM,
    STATE_TRADE_DATA,
    STATE_TRADE_PATCH_HEADER,
    STATE_TRADE_PATCH_DATA,
    STATE_TRADE_MAIL,
    STATE_TRADE_SELECT,
    STATE_TRADE_PENDING,
    STATE_TRADE_CONFIRMATION,
    STATE_TRADE_DONE,
    STATE_TRADE_CANCEL
};

enum GBVisualStatus {
    STATUS_WAITING_GB,
    STATUS_LINK_CONNECTED,
    STATUS_IN_TRADE_ROOM,
    STATUS_EXCHANGING_DATA,
    STATUS_SELECTING_POKEMON,
    STATUS_CONFIRMING_DEAL,
    STATUS_TRADING_ANIMATION,
    STATUS_TRADE_SUCCESS
};

typedef void (*trade_event_callback_t)(GBVisualStatus status, const char* message);

class GBTradeEngine {
public:
    GBTradeEngine();
    ~GBTradeEngine();

    void begin(uint8_t gen, uint8_t clk_pin, uint8_t so_pin, uint8_t si_pin);
    void setGeneration(uint8_t gen);
    uint8_t getGeneration() const { return _gen; }

    GBVisualStatus getVisualStatus() const { return _visual_status; }
    const char* getVisualStatusString() const;

    PokemonData* getOutgoingPokemonData() { return _pdata; }
    PokemonData* getReceivedPokemonData() { return _received_pdata; }

    void setEventCallback(trade_event_callback_t cb) { _event_cb = cb; }

    void configureOutgoingPokemon(
        uint8_t species_num,
        uint8_t level,
        const char* nickname,
        const char* ot_name,
        uint16_t ot_id,
        uint8_t move0,
        uint8_t move1,
        uint8_t move2,
        uint8_t move3,
        EvIvPreset eviv_preset,
        bool shiny,
        uint8_t held_item
    );

    void prepareReturnTradedPokemon(
        uint8_t level,
        const char* nickname,
        uint8_t move0,
        uint8_t move1,
        uint8_t move2,
        uint8_t move3,
        bool shiny,
        uint8_t held_item
    );

    void rebuildPatchList();

    // The core SPI byte transfer processor called by the Link Driver ISR
    uint8_t onByteExchange(uint8_t in_byte);

private:
    uint8_t _gen;
    PokemonData* _pdata;
    PokemonData* _received_pdata;
    PatchList* _patch_list;

    GBTradeState _trade_state;
    volatile GBVisualStatus _visual_status;

    trade_event_callback_t _event_cb;

    size_t _counter;
    bool _patch_pt_2;
    uint8_t _incoming_selected_index;
    uint32_t _last_status_change_ms;

    uint8_t handleConnectPhase(uint8_t in_byte);
    uint8_t handleMenuPhase(uint8_t in_byte);
    uint8_t handleTradeCentrePhase(uint8_t in_byte);

    void updateStatus(GBVisualStatus new_status, const char* msg);
};

extern GBTradeEngine TradeEngine;
