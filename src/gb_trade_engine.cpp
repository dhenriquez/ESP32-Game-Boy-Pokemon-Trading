#include <esp_attr.h>
#include "gb_trade_engine.h"

#define PKMN_BLANK 0x00
#define PKMN_MASTER 0x01
#define PKMN_SLAVE 0x02
#define PKMN_CONNECTED 0x60
#define PKMN_CONNECTED_II 0x61

#define ITEM_1_HIGHLIGHTED 0xD0
#define ITEM_2_HIGHLIGHTED 0xD1
#define ITEM_3_HIGHLIGHTED 0xD2
#define ITEM_1_SELECTED 0xD4
#define ITEM_2_SELECTED 0xD5
#define ITEM_3_SELECTED 0xD6

#define PKMN_TRADE_CENTRE ITEM_1_SELECTED
#define PKMN_COLOSSEUM ITEM_2_SELECTED
#define PKMN_BREAK_LINK ITEM_3_SELECTED

#define SERIAL_PREAMBLE_BYTE 0xFD
#define SERIAL_RNS_LENGTH 10
#define SERIAL_TRADE_PREAMBLE_LENGTH 9
#define SERIAL_PATCH_LIST_PART_TERMINATOR 0xFF
#define SERIAL_NO_DATA_BYTE 0xFE

#define PKMN_TRADE_ACCEPT_GEN_I 0x62
#define PKMN_TRADE_ACCEPT_GEN_II 0x72
#define PKMN_TRADE_REJECT_GEN_I 0x61
#define PKMN_TRADE_REJECT_GEN_II 0x71
#define PKMN_TABLE_LEAVE_GEN_I 0x6F
#define PKMN_TABLE_LEAVE_GEN_II 0x7F
#define PKMN_SEL_NUM_MASK_GEN_I 0x60
#define PKMN_SEL_NUM_MASK_GEN_II 0x70
#define PKMN_SEL_NUM_ONE_GEN_I 0x60
#define PKMN_SEL_NUM_ONE_GEN_II 0x70

struct ImportantBytes {
    uint8_t connected;
    uint8_t trade_accept;
    uint8_t trade_reject;
    uint8_t table_leave;
    uint8_t sel_num_mask;
    uint8_t sel_num_one;
};

static const DRAM_ATTR ImportantBytes GEN_I_BYTES = {
    PKMN_CONNECTED,
    PKMN_TRADE_ACCEPT_GEN_I,
    PKMN_TRADE_REJECT_GEN_I,
    PKMN_TABLE_LEAVE_GEN_I,
    PKMN_SEL_NUM_MASK_GEN_I,
    PKMN_SEL_NUM_ONE_GEN_I,
};

static const DRAM_ATTR ImportantBytes GEN_II_BYTES = {
    PKMN_CONNECTED_II,
    PKMN_TRADE_ACCEPT_GEN_II,
    PKMN_TRADE_REJECT_GEN_II,
    PKMN_TABLE_LEAVE_GEN_II,
    PKMN_SEL_NUM_MASK_GEN_II,
    PKMN_SEL_NUM_ONE_GEN_II,
};

GBTradeEngine TradeEngine;

static uint8_t IRAM_ATTR driver_callback_shim(void* context, uint8_t in_byte) {
    GBTradeEngine* engine = (GBTradeEngine*)context;
    if (!engine) return PKMN_BLANK;
    return engine->onByteExchange(in_byte);
}

GBTradeEngine::GBTradeEngine()
    : _gen(GEN_I)
    , _pdata(NULL)
    , _received_pdata(NULL)
    , _patch_list(NULL)
    , _trade_state(STATE_NOT_CONNECTED)
    , _visual_status(STATUS_WAITING_GB)
    , _event_cb(NULL)
    , _gen_cb(NULL)
    , _received_cb(NULL)
    , _status_pending(false)
    , _pending_status(STATUS_WAITING_GB)
    , _pending_msg(NULL)
    , _pending_gen_switch(0)
    , _received_pending(false)
    , _rebuild_patch_pending(false)
    , _counter(0)
    , _patch_pt_2(false)
    , _incoming_selected_index(0)
    , _last_status_change_ms(0)
{
}

GBTradeEngine::~GBTradeEngine() {
    if (_pdata) pokemon_data_free(_pdata);
    if (_received_pdata) pokemon_data_free(_received_pdata);
    if (_patch_list) plist_free(_patch_list);
}

void GBTradeEngine::begin(uint8_t gen, uint8_t clk_pin, uint8_t so_pin, uint8_t si_pin) {
    _gen = gen;
    _pdata = pokemon_data_alloc(_gen);
    _received_pdata = pokemon_data_alloc(_gen);
    rebuildPatchList();

    GBLink.begin(clk_pin, so_pin, si_pin);
    GBLink.setCallback(driver_callback_shim, this);
    GBLink.setNoDataByte(SERIAL_NO_DATA_BYTE);
    GBLink.start();

    updateStatus(STATUS_WAITING_GB, "En espera de conexión Game Boy...");
}

void GBTradeEngine::setGeneration(uint8_t gen, bool reset_link) {
    if (_gen == gen) return;
    _gen = gen;
    if (_pdata) pokemon_data_free(_pdata);
    if (_received_pdata) pokemon_data_free(_received_pdata);

    _pdata = pokemon_data_alloc(_gen);
    _received_pdata = pokemon_data_alloc(_gen);
    rebuildPatchList();

    if (reset_link) {
        _trade_state = STATE_NOT_CONNECTED;
        updateStatus(STATUS_WAITING_GB, "Generación cambiada. Reiniciando enlace...");
    }

    if (_gen_cb) {
        _gen_cb(_gen);
    }
}

void GBTradeEngine::rebuildPatchList() {
    if (_pdata) {
        plist_create(&_patch_list, _pdata);
    }
}

void GBTradeEngine::configureOutgoingPokemon(
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
) {
    if (!_pdata) return;

    pokemon_stat_set(_pdata, STAT_NUM, NONE, species_num);
    pokemon_stat_set(_pdata, STAT_LEVEL, NONE, level);
    pokemon_stat_set(_pdata, STAT_SEL, NONE, (uint16_t)eviv_preset);

    if (nickname && strlen(nickname) > 0) {
        pokemon_name_set(_pdata, STAT_NICKNAME, nickname);
    } else {
        pokemon_default_nickname_set(NULL, _pdata, 0);
    }

    if (ot_name && strlen(ot_name) > 0) {
        pokemon_name_set(_pdata, STAT_OT_NAME, ot_name);
    }
    pokemon_stat_set(_pdata, STAT_OT_ID, NONE, ot_id);

    pokemon_stat_set(_pdata, STAT_MOVE, MOVE_0, move0);
    pokemon_stat_set(_pdata, STAT_MOVE, MOVE_1, move1);
    pokemon_stat_set(_pdata, STAT_MOVE, MOVE_2, move2);
    pokemon_stat_set(_pdata, STAT_MOVE, MOVE_3, move3);

    if (_gen == GEN_II) {
        pokemon_stat_set(_pdata, STAT_HELD_ITEM, NONE, held_item);
        pokemon_set_shiny(_pdata, shiny);
    }

    rebuildPatchList();
}

void GBTradeEngine::prepareReturnTradedPokemon(
    uint8_t level,
    const char* nickname,
    uint8_t move0,
    uint8_t move1,
    uint8_t move2,
    uint8_t move3,
    bool shiny,
    uint8_t held_item
) {
    if (!_pdata || !_received_pdata) return;

    // Copy received pokemon to outgoing
    pokemon_stat_memcpy(_pdata, _received_pdata, 0);

    if (level > 0 && level <= 100) {
        pokemon_stat_set(_pdata, STAT_LEVEL, NONE, level);
    }
    if (nickname && strlen(nickname) > 0) {
        pokemon_name_set(_pdata, STAT_NICKNAME, nickname);
    }
    if (move0 != 0) pokemon_stat_set(_pdata, STAT_MOVE, MOVE_0, move0);
    if (move1 != 0) pokemon_stat_set(_pdata, STAT_MOVE, MOVE_1, move1);
    if (move2 != 0) pokemon_stat_set(_pdata, STAT_MOVE, MOVE_2, move2);
    if (move3 != 0) pokemon_stat_set(_pdata, STAT_MOVE, MOVE_3, move3);

    if (_gen == GEN_II) {
        pokemon_stat_set(_pdata, STAT_HELD_ITEM, NONE, held_item);
        if (shiny) pokemon_set_shiny(_pdata, true);
    }

    rebuildPatchList();
}

const char* GBTradeEngine::getVisualStatusString() const {
    switch (_visual_status) {
    case STATUS_WAITING_GB: return "Esperando Game Boy...";
    case STATUS_LINK_CONNECTED: return "Conectado al Game Boy";
    case STATUS_IN_TRADE_ROOM: return "En la Sala de Intercambio";
    case STATUS_EXCHANGING_DATA: return "Transfiriendo datos de equipo...";
    case STATUS_SELECTING_POKEMON: return "Seleccionando Pokémon...";
    case STATUS_CONFIRMING_DEAL: return "Esperando confirmación de trato...";
    case STATUS_TRADING_ANIMATION: return "Realizando animación de trade...";
    case STATUS_TRADE_SUCCESS: return "¡Intercambio completado!";
    default: return "Desconocido";
    }
}

void GBTradeEngine::process() {
    if (_pending_gen_switch != 0) {
        uint8_t target_gen = _pending_gen_switch;
        _pending_gen_switch = 0;
        setGeneration(target_gen, false);
    }

    if (_rebuild_patch_pending) {
        _rebuild_patch_pending = false;
        rebuildPatchList();
    }

    if (_status_pending) {
        _status_pending = false;
        GBVisualStatus st = (GBVisualStatus)_pending_status;
        const char* msg = (const char*)_pending_msg;
        _last_status_change_ms = millis();
        if (_event_cb) {
            _event_cb(st, msg);
        }
    }

    if (_received_pending) {
        _received_pending = false;
        if (_received_cb) {
            _received_cb(_received_pdata);
        }
    }
}

void IRAM_ATTR GBTradeEngine::updateStatus(GBVisualStatus new_status, const char* msg) {
    _visual_status = new_status;
    _pending_status = new_status;
    _pending_msg = msg;
    _status_pending = true;
}

uint8_t IRAM_ATTR GBTradeEngine::onByteExchange(uint8_t in_byte) {
    switch (_trade_state) {
    case STATE_NOT_CONNECTED:
        return handleConnectPhase(in_byte);
    case STATE_CONNECTED:
        return handleMenuPhase(in_byte);
    default:
        return handleTradeCentrePhase(in_byte);
    }
}

uint8_t IRAM_ATTR GBTradeEngine::handleConnectPhase(uint8_t in_byte) {
    uint8_t response = PKMN_BLANK;
    switch (in_byte) {
    case PKMN_MASTER:
        response = PKMN_SLAVE;
        break;
    case PKMN_BLANK:
        response = PKMN_BLANK;
        break;
    case PKMN_CONNECTED:
        response = in_byte;
        _trade_state = STATE_CONNECTED;
        if (_gen != GEN_I) {
            _pending_gen_switch = GEN_I;
        }
        updateStatus(STATUS_LINK_CONNECTED, "Conexión Game Boy detectada (Gen I).");
        break;
    case PKMN_CONNECTED_II:
        response = in_byte;
        _trade_state = STATE_CONNECTED;
        if (_gen != GEN_II) {
            _pending_gen_switch = GEN_II;
        }
        updateStatus(STATUS_LINK_CONNECTED, "Conexión Game Boy detectada (Gen II).");
        break;
    default:
        response = PKMN_BLANK;
        break;
    }
    return response;
}

uint8_t IRAM_ATTR GBTradeEngine::handleMenuPhase(uint8_t in_byte) {
    uint8_t response = PKMN_BLANK;
    switch (in_byte) {
    case PKMN_CONNECTED:
    case PKMN_CONNECTED_II:
        response = in_byte;
        break;
    case ITEM_2_HIGHLIGHTED:
        if (_gen == GEN_I) {
            response = in_byte;
            break;
        }
        [[fallthrough]];
    case PKMN_TRADE_CENTRE:
        _trade_state = STATE_TRADE_INIT;
        _counter = 0;
        _patch_pt_2 = false;
        updateStatus(STATUS_IN_TRADE_ROOM, "En la Sala de Intercambio (Trade Center).");
        break;
    case PKMN_BREAK_LINK:
    case PKMN_MASTER:
        _trade_state = STATE_NOT_CONNECTED;
        response = PKMN_BREAK_LINK;
        updateStatus(STATUS_WAITING_GB, "Enlace reiniciado por Game Boy.");
        break;
    default:
        response = in_byte;
        break;
    }
    return response;
}

uint8_t IRAM_ATTR GBTradeEngine::handleTradeCentrePhase(uint8_t in_byte) {
    uint8_t* trade_block_flat = (uint8_t*)_pdata->trade_block;
    uint8_t* input_block_flat = (uint8_t*)_received_pdata->trade_block;
    uint8_t* input_party_flat = (uint8_t*)_received_pdata->party;
    uint8_t send = in_byte;

    const ImportantBytes* bytes = (_gen == GEN_I) ? &GEN_I_BYTES : &GEN_II_BYTES;

    switch (_trade_state) {
    case STATE_TRADE_INIT:
        if (in_byte == SERIAL_PREAMBLE_BYTE) {
            _counter++;
            updateStatus(STATUS_EXCHANGING_DATA, "Sincronizando preámbulo...");
        }
        if (_counter == SERIAL_RNS_LENGTH) {
            _trade_state = STATE_TRADE_RANDOM;
            _counter = 0;
        }
        break;

    case STATE_TRADE_RANDOM:
        _counter++;
        if (_counter == (SERIAL_RNS_LENGTH + SERIAL_TRADE_PREAMBLE_LENGTH)) {
            _trade_state = STATE_TRADE_DATA;
            _counter = 0;
            updateStatus(STATUS_EXCHANGING_DATA, "Transfiriendo bloques de Pokémon...");
        }
        break;

    case STATE_TRADE_DATA:
        input_block_flat[_counter] = in_byte;
        send = trade_block_flat[_counter];
        _counter++;

        if (_counter == _pdata->trade_block_sz) {
            _trade_state = STATE_TRADE_PATCH_HEADER;
            _counter = 0;
        }
        break;

    case STATE_TRADE_PATCH_HEADER:
        if (in_byte == SERIAL_PREAMBLE_BYTE) {
            _counter++;
        }
        if (_counter == 6) {
            _counter = 0;
            _patch_pt_2 = false;
            _trade_state = STATE_TRADE_PATCH_DATA;
        }
        break;

    case STATE_TRADE_PATCH_DATA:
        _counter++;
        if (_counter > 8) {
            send = plist_index_get(_patch_list, _counter - 9);
        }

        switch (in_byte) {
        case PKMN_BLANK:
            break;
        case SERIAL_PATCH_LIST_PART_TERMINATOR:
            _patch_pt_2 = true;
            break;
        default:
            if (!_patch_pt_2) {
                if (in_byte > 0 && in_byte <= 0xFC) {
                    input_party_flat[in_byte - 1] = SERIAL_NO_DATA_BYTE;
                }
            } else {
                if (in_byte > 0) {
                    input_party_flat[0xFB + in_byte] = SERIAL_NO_DATA_BYTE;
                }
            }
            break;
        }

        if (_counter == 196) {
            _counter = 0;
            if (_gen == GEN_I) {
                _trade_state = STATE_TRADE_SELECT;
                updateStatus(STATUS_SELECTING_POKEMON, "Listo en la mesa de trade.");
            } else {
                _trade_state = STATE_TRADE_MAIL;
            }
        }
        break;

    case STATE_TRADE_MAIL:
        _counter++;
        if (_counter == 389) {
            _counter = 0;
            _trade_state = STATE_TRADE_SELECT;
            updateStatus(STATUS_SELECTING_POKEMON, "Listo en la mesa de trade.");
        }
        break;

    case STATE_TRADE_SELECT:
        _incoming_selected_index = 0;
        if (in_byte == PKMN_BLANK) {
            _trade_state = STATE_TRADE_PENDING;
        }
        break;

    case STATE_TRADE_PENDING:
        if (in_byte == bytes->table_leave) {
            _trade_state = STATE_TRADE_INIT;
            send = bytes->table_leave;
            updateStatus(STATUS_IN_TRADE_ROOM, "El jugador salió de la mesa.");
        } else if ((in_byte & bytes->sel_num_mask) == bytes->sel_num_mask) {
            _incoming_selected_index = in_byte;
            send = bytes->sel_num_one; // We offer our 1st slot
            updateStatus(STATUS_SELECTING_POKEMON, "Oferta de intercambio iniciada.");
        } else if (in_byte == PKMN_BLANK) {
            if (_incoming_selected_index != 0) {
                send = 0;
                _trade_state = STATE_TRADE_CONFIRMATION;
                _incoming_selected_index &= 0x0F;
                updateStatus(STATUS_CONFIRMING_DEAL, "Esperando confirmación final...");
            }
        }
        break;

    case STATE_TRADE_CONFIRMATION:
        if (in_byte == bytes->trade_reject) {
            _trade_state = STATE_TRADE_SELECT;
            updateStatus(STATUS_SELECTING_POKEMON, "Trato rechazado. Volviendo a selección.");
        } else if (in_byte == bytes->trade_accept) {
            _trade_state = STATE_TRADE_DONE;
            updateStatus(STATUS_TRADING_ANIMATION, "¡Trato aceptado! Intercambiando...");
        }
        break;

    case STATE_TRADE_DONE:
        if (in_byte == PKMN_BLANK) {
            _trade_state = STATE_TRADE_INIT;
            _counter = 0;

            // 1. Move incoming pokemon from selected party slot to slot 0 of _received_pdata
            if (_incoming_selected_index != 0) {
                pokemon_stat_memcpy(_received_pdata, _received_pdata, _incoming_selected_index);
            }

            // 2. Adopt incoming pokemon into _pdata slot 0 with its exact moves and stats
            pokemon_stat_memcpy(_pdata, _received_pdata, 0);

            _rebuild_patch_pending = true;
            _received_pending = true;
            updateStatus(STATUS_TRADE_SUCCESS, "¡Intercambio completado exitosamente!");
        }
        break;

    case STATE_TRADE_CANCEL:
        if (in_byte == bytes->table_leave) {
            _trade_state = STATE_TRADE_INIT;
            updateStatus(STATUS_IN_TRADE_ROOM, "Intercambio cancelado.");
        }
        send = bytes->table_leave;
        break;

    default:
        break;
    }

    return send;
}
