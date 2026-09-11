#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "pokemon_data.h"
#include "pokemon_char_encode.h"
#include "pokemon_table.h"
#include "named_list.h"
#include "move_nl.h"
#include "item_nl.h"
#include "type_nl.h"
#include "stat_nl.h"

#define RECALC_NONE 0x00
#define RECALC_EXP 0x01
#define RECALC_EVS 0x02
#define RECALC_IVS 0x04
#define RECALC_STATS 0x08
#define RECALC_NICKNAME 0x10
#define RECALC_MOVES 0x20
#define RECALC_TYPES 0x40
#define RECALC_ALL 0xFF

static void pokemon_recalculate(PokemonData* pdata, uint8_t recalc);
static void pokemon_stat_ev_calc(PokemonData* pdata, EvIvPreset val);
static void pokemon_stat_iv_calc(PokemonData* pdata, EvIvPreset val);

PokemonData* pokemon_data_alloc(uint8_t gen) {
    PokemonData* pdata = (PokemonData*)malloc(sizeof(PokemonData));
    if (!pdata) return NULL;
    memset(pdata, 0, sizeof(PokemonData));

    pdata->gen = gen;
    pdata->move_list = move_list;
    pdata->type_list = type_list;
    pdata->stat_list = stat_list;
    pdata->item_list = item_list;
    pdata->pokemon_table = table_pointer_get();
    pdata->stat_sel = MAXIV_MAXEV;

    switch(gen) {
    case GEN_I:
        pdata->trade_block_sz = sizeof(TradeBlockGenI); // 415
        pdata->party_sz = sizeof(PokemonPartyGenI) * 6;  // 264
        pdata->trade_block = malloc(pdata->trade_block_sz);
        memset(pdata->trade_block, 0, pdata->trade_block_sz);

        memset(((TradeBlockGenI*)pdata->trade_block)->party_members, 0xFF, sizeof(((TradeBlockGenI*)pdata->trade_block)->party_members));
        pdata->party = ((TradeBlockGenI*)pdata->trade_block)->party;
        ((TradeBlockGenI*)pdata->trade_block)->party_cnt = 1;
        pdata->dex_max = 150;
        break;

    case GEN_II:
    default:
        pdata->gen = GEN_II;
        pdata->trade_block_sz = sizeof(TradeBlockGenII); // 441
        pdata->party_sz = sizeof(PokemonPartyGenII) * 6;  // 288
        pdata->trade_block = malloc(pdata->trade_block_sz);
        memset(pdata->trade_block, 0, pdata->trade_block_sz);

        memset(((TradeBlockGenII*)pdata->trade_block)->party_members, 0xFF, sizeof(((TradeBlockGenII*)pdata->trade_block)->party_members));
        pdata->party = ((TradeBlockGenII*)pdata->trade_block)->party;
        ((TradeBlockGenII*)pdata->trade_block)->party_cnt = 1;
        pdata->dex_max = 250;
        break;
    }

    pokemon_name_set(pdata, STAT_TRAINER_NAME, "ESP32");
    pokemon_name_set(pdata, STAT_OT_NAME, "ESP32");
    pokemon_stat_set(pdata, STAT_OT_ID, NONE, 42069);

    pokemon_stat_set(pdata, STAT_NUM, NONE, 0); // Bulbasaur / Chikorita
    pokemon_stat_set(pdata, STAT_LEVEL, NONE, 5);

    return pdata;
}

void pokemon_data_free(PokemonData* pdata) {
    if (!pdata) return;
    if (pdata->trade_block) {
        free(pdata->trade_block);
        pdata->trade_block = NULL;
    }
    free(pdata);
}

void pokemon_name_set(PokemonData* pdata, DataStat stat, const char* name) {
    if (!pdata || !pdata->trade_block) return;
    size_t len = strlen(name);
    uint8_t gen = pdata->gen;
    uint8_t* ptr = NULL;

    switch(stat) {
    case STAT_NICKNAME:
        if (gen == GEN_I) ptr = ((TradeBlockGenI*)pdata->trade_block)->nickname[0].str;
        if (gen == GEN_II) ptr = ((TradeBlockGenII*)pdata->trade_block)->nickname[0].str;
        len = (len > 10) ? 10 : len;
        break;
    case STAT_OT_NAME:
        if (gen == GEN_I) ptr = ((TradeBlockGenI*)pdata->trade_block)->ot_name[0].str;
        if (gen == GEN_II) ptr = ((TradeBlockGenII*)pdata->trade_block)->ot_name[0].str;
        len = (len > 7) ? 7 : len;
        break;
    case STAT_TRAINER_NAME:
        if (gen == GEN_I) ptr = ((TradeBlockGenI*)pdata->trade_block)->trainer_name.str;
        if (gen == GEN_II) ptr = ((TradeBlockGenII*)pdata->trade_block)->trainer_name.str;
        len = (len > 7) ? 7 : len;
        break;
    default:
        return;
    }

    memset(ptr, TERM_, LEN_NAME_BUF);
    pokemon_str_to_encoded_array(ptr, name, len);
}

void pokemon_name_get(PokemonData* pdata, DataStat stat, char* dest, size_t len) {
    if (!pdata || !pdata->trade_block || !dest || len == 0) return;
    uint8_t* ptr = NULL;
    uint8_t gen = pdata->gen;

    switch(stat) {
    case STAT_NICKNAME:
        if (gen == GEN_I) ptr = ((TradeBlockGenI*)pdata->trade_block)->nickname[0].str;
        if (gen == GEN_II) ptr = ((TradeBlockGenII*)pdata->trade_block)->nickname[0].str;
        break;
    case STAT_OT_NAME:
        if (gen == GEN_I) ptr = ((TradeBlockGenI*)pdata->trade_block)->ot_name[0].str;
        if (gen == GEN_II) ptr = ((TradeBlockGenII*)pdata->trade_block)->ot_name[0].str;
        break;
    case STAT_TRAINER_NAME:
        if (gen == GEN_I) ptr = ((TradeBlockGenI*)pdata->trade_block)->trainer_name.str;
        if (gen == GEN_II) ptr = ((TradeBlockGenII*)pdata->trade_block)->trainer_name.str;
        break;
    default:
        dest[0] = '\0';
        return;
    }

    pokemon_encoded_array_to_str(dest, ptr, len);
}

void pokemon_default_nickname_set(char* dest, PokemonData* pdata, size_t n) {
    if (!pdata) return;
    char buf[LEN_NAME_BUF];
    const char* name = table_stat_name_get(pdata->pokemon_table, pokemon_stat_get(pdata, STAT_NUM, NONE));
    strncpy(buf, name, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    for (size_t i = 0; i < strlen(buf); i++) {
        buf[i] = toupper((unsigned char)buf[i]);
    }

    pokemon_name_set(pdata, STAT_NICKNAME, buf);
    if (dest && n > 0) {
        strncpy(dest, buf, n - 1);
        dest[n - 1] = '\0';
    }
}

uint16_t pokemon_stat_get(PokemonData* pdata, DataStat stat, DataStatSub which) {
    if (!pdata || !pdata->party) return 0;
    void* party = pdata->party;
    int gen = pdata->gen;
    uint16_t val = 0;
    uint8_t hp_iv = 0;

    switch(stat) {
    case STAT_ATK:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->atk : ((PokemonPartyGenII*)party)->atk;
        break;
    case STAT_DEF:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->def : ((PokemonPartyGenII*)party)->def;
        break;
    case STAT_SPD:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->spd : ((PokemonPartyGenII*)party)->spd;
        break;
    case STAT_SPC:
        if (gen == GEN_I) val = ((PokemonPartyGenI*)party)->spc;
        break;
    case STAT_SPC_ATK:
        if (gen == GEN_II) val = ((PokemonPartyGenII*)party)->spc_atk;
        break;
    case STAT_SPC_DEF:
        if (gen == GEN_II) val = ((PokemonPartyGenII*)party)->spc_def;
        break;
    case STAT_HP:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->hp : ((PokemonPartyGenII*)party)->hp;
        break;
    case STAT_ATK_EV:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->atk_ev : ((PokemonPartyGenII*)party)->atk_ev;
        break;
    case STAT_DEF_EV:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->def_ev : ((PokemonPartyGenII*)party)->def_ev;
        break;
    case STAT_SPD_EV:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->spd_ev : ((PokemonPartyGenII*)party)->spd_ev;
        break;
    case STAT_SPC_EV:
    case STAT_SPC_ATK_EV:
    case STAT_SPC_DEF_EV:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->spc_ev : ((PokemonPartyGenII*)party)->spc_ev;
        break;
    case STAT_HP_EV:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->hp_ev : ((PokemonPartyGenII*)party)->hp_ev;
        break;
    case STAT_IV:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->iv : ((PokemonPartyGenII*)party)->iv;
        break;
    case STAT_SPD_IV:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->iv : ((PokemonPartyGenII*)party)->iv;
        return (val >> 12) & 0x0F;
    case STAT_SPC_IV:
    case STAT_SPC_ATK_IV:
    case STAT_SPC_DEF_IV:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->iv : ((PokemonPartyGenII*)party)->iv;
        return (val >> 8) & 0x0F;
    case STAT_ATK_IV:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->iv : ((PokemonPartyGenII*)party)->iv;
        return (val >> 4) & 0x0F;
    case STAT_DEF_IV:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->iv : ((PokemonPartyGenII*)party)->iv;
        return val & 0x0F;
    case STAT_HP_IV:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->iv : ((PokemonPartyGenII*)party)->iv;
        hp_iv |= ((val & 0x0010) >> 1);
        hp_iv |= ((val & 0x0001) << 2);
        hp_iv |= ((val & 0x1000) >> 11);
        hp_iv |= ((val & 0x0100) >> 8);
        return hp_iv;
    case STAT_LEVEL:
        return (gen == GEN_I) ? ((PokemonPartyGenI*)party)->level : ((PokemonPartyGenII*)party)->level;
    case STAT_INDEX:
        return (gen == GEN_I) ? ((PokemonPartyGenI*)party)->index : (((PokemonPartyGenII*)party)->index - 1);
    case STAT_NUM:
        if (gen == GEN_I) {
            return table_pokemon_pos_get(pdata->pokemon_table, ((PokemonPartyGenI*)party)->index);
        }
        return ((PokemonPartyGenII*)party)->index - 1;
    case STAT_MOVE:
        return (gen == GEN_I) ? ((PokemonPartyGenI*)party)->move[which] : ((PokemonPartyGenII*)party)->move[which];
    case STAT_TYPE:
        return (gen == GEN_I) ? ((PokemonPartyGenI*)party)->type[which] : 0;
    case STAT_OT_ID:
        val = (gen == GEN_I) ? ((PokemonPartyGenI*)party)->ot_id : ((PokemonPartyGenII*)party)->ot_id;
        break;
    case STAT_POKERUS:
        return (gen == GEN_II) ? ((PokemonPartyGenII*)party)->pokerus : 0;
    case STAT_SEL:
        return (uint16_t)pdata->stat_sel;
    case STAT_CONDITION:
        return (gen == GEN_I) ? ((PokemonPartyGenI*)party)->status_condition : ((PokemonPartyGenII*)party)->status_condition;
    case STAT_HELD_ITEM:
        return (gen == GEN_II) ? ((PokemonPartyGenII*)party)->held_item : 0;
    default:
        return 0;
    }

    return __builtin_bswap16(val);
}

void pokemon_stat_set(PokemonData* pdata, DataStat stat, DataStatSub which, uint16_t val) {
    if (!pdata || !pdata->party) return;
    void* party = pdata->party;
    int gen = pdata->gen;
    uint8_t recalc = 0;
    uint16_t val_swap = __builtin_bswap16(val);

    switch(stat) {
    case STAT_ATK:
        if (gen == GEN_I) ((PokemonPartyGenI*)party)->atk = val_swap;
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->atk = val_swap;
        break;
    case STAT_DEF:
        if (gen == GEN_I) ((PokemonPartyGenI*)party)->def = val_swap;
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->def = val_swap;
        break;
    case STAT_SPD:
        if (gen == GEN_I) ((PokemonPartyGenI*)party)->spd = val_swap;
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->spd = val_swap;
        break;
    case STAT_SPC:
        if (gen == GEN_I) ((PokemonPartyGenI*)party)->spc = val_swap;
        break;
    case STAT_SPC_ATK:
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->spc_atk = val_swap;
        break;
    case STAT_SPC_DEF:
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->spc_def = val_swap;
        break;
    case STAT_HP:
        if (gen == GEN_I) {
            ((PokemonPartyGenI*)party)->hp = val_swap;
            ((PokemonPartyGenI*)party)->max_hp = val_swap;
        }
        if (gen == GEN_II) {
            ((PokemonPartyGenII*)party)->hp = val_swap;
            ((PokemonPartyGenII*)party)->max_hp = val_swap;
        }
        break;
    case STAT_ATK_EV:
        if (gen == GEN_I) ((PokemonPartyGenI*)party)->atk_ev = val_swap;
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->atk_ev = val_swap;
        break;
    case STAT_DEF_EV:
        if (gen == GEN_I) ((PokemonPartyGenI*)party)->def_ev = val_swap;
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->def_ev = val_swap;
        break;
    case STAT_SPD_EV:
        if (gen == GEN_I) ((PokemonPartyGenI*)party)->spd_ev = val_swap;
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->spd_ev = val_swap;
        break;
    case STAT_SPC_EV:
    case STAT_SPC_ATK_EV:
    case STAT_SPC_DEF_EV:
        if (gen == GEN_I) ((PokemonPartyGenI*)party)->spc_ev = val_swap;
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->spc_ev = val_swap;
        break;
    case STAT_HP_EV:
        if (gen == GEN_I) ((PokemonPartyGenI*)party)->hp_ev = val_swap;
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->hp_ev = val_swap;
        break;
    case STAT_IV:
        if (gen == GEN_I) ((PokemonPartyGenI*)party)->iv = val;
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->iv = val;
        break;
    case STAT_SPD_IV:
        if (gen == GEN_I) {
            ((PokemonPartyGenI*)party)->iv &= ~(0x0F << 12);
            ((PokemonPartyGenI*)party)->iv |= ((val & 0x0F) << 12);
        }
        if (gen == GEN_II) {
            ((PokemonPartyGenII*)party)->iv &= ~(0x0F << 12);
            ((PokemonPartyGenII*)party)->iv |= ((val & 0x0F) << 12);
        }
        break;
    case STAT_SPC_IV:
    case STAT_SPC_ATK_IV:
    case STAT_SPC_DEF_IV:
        if (gen == GEN_I) {
            ((PokemonPartyGenI*)party)->iv &= ~(0x0F << 8);
            ((PokemonPartyGenI*)party)->iv |= ((val & 0x0F) << 8);
        }
        if (gen == GEN_II) {
            ((PokemonPartyGenII*)party)->iv &= ~(0x0F << 8);
            ((PokemonPartyGenII*)party)->iv |= ((val & 0x0F) << 8);
        }
        break;
    case STAT_ATK_IV:
        if (gen == GEN_I) {
            ((PokemonPartyGenI*)party)->iv &= ~(0x0F << 4);
            ((PokemonPartyGenI*)party)->iv |= ((val & 0x0F) << 4);
        }
        if (gen == GEN_II) {
            ((PokemonPartyGenII*)party)->iv &= ~(0x0F << 4);
            ((PokemonPartyGenII*)party)->iv |= ((val & 0x0F) << 4);
        }
        break;
    case STAT_DEF_IV:
        if (gen == GEN_I) {
            ((PokemonPartyGenI*)party)->iv &= ~(0x0F);
            ((PokemonPartyGenI*)party)->iv |= (val & 0x0F);
        }
        if (gen == GEN_II) {
            ((PokemonPartyGenII*)party)->iv &= ~(0x0F);
            ((PokemonPartyGenII*)party)->iv |= (val & 0x0F);
        }
        break;
    case STAT_MOVE:
        if (gen == GEN_I) ((PokemonPartyGenI*)party)->move[which] = val;
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->move[which] = val;
        break;
    case STAT_TYPE:
        if (gen == GEN_I) ((PokemonPartyGenI*)party)->type[which] = val;
        break;
    case STAT_LEVEL:
        if (gen == GEN_I) {
            ((PokemonPartyGenI*)party)->level = val;
            ((PokemonPartyGenI*)party)->level_again = val;
        }
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->level = val;
        recalc = (RECALC_STATS | RECALC_EXP | RECALC_EVS);
        break;
    case STAT_INDEX:
        if (gen == GEN_I) {
            ((PokemonPartyGenI*)party)->index = val;
            ((TradeBlockGenI*)pdata->trade_block)->party_members[0] = val;
        }
        if (gen == GEN_II) {
            ((PokemonPartyGenII*)party)->index = val + 1;
            ((TradeBlockGenII*)pdata->trade_block)->party_members[0] = val + 1;
        }
        recalc = RECALC_ALL;
        break;
    case STAT_NUM:
        if (gen == GEN_I) {
            pokemon_stat_set(pdata, STAT_INDEX, NONE, table_stat_base_get(pdata->pokemon_table, val, STAT_BASE_INDEX, NONE));
        }
        if (gen == GEN_II) {
            pokemon_stat_set(pdata, STAT_INDEX, NONE, val);
        }
        break;
    case STAT_OT_ID:
        if (gen == GEN_I) ((PokemonPartyGenI*)party)->ot_id = val_swap;
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->ot_id = val_swap;
        break;
    case STAT_POKERUS:
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->pokerus = val;
        break;
    case STAT_SEL:
        pdata->stat_sel = (EvIvPreset)val;
        recalc = (RECALC_EVS | RECALC_IVS | RECALC_STATS);
        break;
    case STAT_EXP:
        if (gen == GEN_I) ((PokemonPartyGenI*)party)->exp[which] = val;
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->exp[which] = val;
        break;
    case STAT_CONDITION:
        if (gen == GEN_I) ((PokemonPartyGenI*)party)->status_condition = val;
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->status_condition = val;
        break;
    case STAT_HELD_ITEM:
        if (gen == GEN_II) ((PokemonPartyGenII*)party)->held_item = val;
        break;
    default:
        break;
    }

    pokemon_recalculate(pdata, recalc);
}

static void pokemon_recalculate(PokemonData* pdata, uint8_t recalc) {
    if (!pdata || recalc == RECALC_NONE) return;

    if (recalc & RECALC_NICKNAME) {
        pokemon_default_nickname_set(NULL, pdata, 0);
    }

    if (recalc & RECALC_MOVES) {
        for (int i = MOVE_0; i <= MOVE_3; i++) {
            pokemon_stat_set(pdata, STAT_MOVE, (DataStatSub)i,
                table_stat_base_get(pdata->pokemon_table, pokemon_stat_get(pdata, STAT_NUM, NONE), STAT_BASE_MOVE, (DataStatSub)i));
        }
    }

    if (recalc & RECALC_TYPES) {
        for (int i = TYPE_0; i <= TYPE_1; i++) {
            pokemon_stat_set(pdata, STAT_TYPE, (DataStatSub)i,
                table_stat_base_get(pdata->pokemon_table, pokemon_stat_get(pdata, STAT_NUM, NONE), STAT_BASE_TYPE, (DataStatSub)i));
        }
    }

    if (recalc & RECALC_EXP) pokemon_exp_calc(pdata);
    if (recalc & RECALC_EVS) pokemon_stat_ev_calc(pdata, pdata->stat_sel);
    if (recalc & RECALC_IVS) pokemon_stat_iv_calc(pdata, pdata->stat_sel);

    if (recalc & RECALC_STATS) {
        for (int i = STAT; i < STAT_END; i++) {
            pokemon_stat_calc(pdata, (DataStat)i);
        }
    }
}

static void pokemon_stat_ev_calc(PokemonData* pdata, EvIvPreset val) {
    if (!pdata) return;
    int level = pokemon_stat_get(pdata, STAT_LEVEL, NONE);
    uint16_t ev;

    switch(val) {
    case RANDIV_LEVELEV:
    case MAXIV_LEVELEV:
        ev = (0xffff / 100) * level;
        break;
    case RANDIV_MAXEV:
    case MAXIV_MAXEV:
        ev = 0xffff;
        break;
    default:
        ev = 0;
        break;
    }

    for (int i = STAT_EV; i < STAT_EV_END; i++) {
        pokemon_stat_set(pdata, (DataStat)i, NONE, ev);
    }
}

static void pokemon_stat_iv_calc(PokemonData* pdata, EvIvPreset val) {
    if (!pdata) return;
    uint16_t iv;

    switch(val) {
    case RANDIV_ZEROEV:
    case RANDIV_LEVELEV:
    case RANDIV_MAXEV:
        iv = (uint16_t)rand();
        break;
    default:
        iv = 0xFFFF;
        break;
    }

    pokemon_stat_set(pdata, STAT_IV, NONE, iv);
}

void pokemon_exp_set(PokemonData* pdata, uint32_t exp) {
    if (!pdata) return;
    uint8_t exp_tmp[3];
    exp_tmp[2] = (uint8_t)(exp & 0xFF);
    exp_tmp[1] = (uint8_t)((exp >> 8) & 0xFF);
    exp_tmp[0] = (uint8_t)((exp >> 16) & 0xFF);

    for (int i = EXP_0; i <= EXP_2; i++) {
        pokemon_stat_set(pdata, STAT_EXP, (DataStatSub)i, exp_tmp[i]);
    }
}

void pokemon_exp_calc(PokemonData* pdata) {
    if (!pdata) return;
    int level = (int)pokemon_stat_get(pdata, STAT_LEVEL, NONE);
    uint8_t growth = table_stat_base_get(pdata->pokemon_table, pokemon_stat_get(pdata, STAT_NUM, NONE), STAT_BASE_GROWTH, NONE);
    uint32_t exp = 0;

    switch(growth) {
    case GROWTH_FAST:
        exp = (4 * level * level * level) / 5;
        break;
    case GROWTH_MEDIUM_FAST:
        exp = (level * level * level);
        break;
    case GROWTH_MEDIUM_SLOW:
        exp = (((level * level * level) * 6 / 5) - (15 * level * level) + (100 * level) - 140);
        break;
    case GROWTH_SLOW:
        exp = (5 * level * level * level) / 4;
        break;
    default:
        exp = (level * level * level);
        break;
    }

    pokemon_exp_set(pdata, exp);
}

void pokemon_stat_calc(PokemonData* pdata, DataStat stat) {
    if (!pdata) return;
    uint8_t level = pokemon_stat_get(pdata, STAT_LEVEL, NONE);
    uint8_t base = table_stat_base_get(pdata->pokemon_table, pokemon_stat_get(pdata, STAT_NUM, NONE), stat, NONE);
    uint16_t ev = pokemon_stat_get(pdata, (DataStat)(stat + STAT_EV_OFFS), NONE);
    uint8_t iv = pokemon_stat_get(pdata, (DataStat)(stat + STAT_IV_OFFS), NONE);

    uint16_t calc = floor((((2 * (base + iv)) + floor(sqrt(ev) / 4.0)) * level) / 100.0);
    if (stat == STAT_HP) {
        calc += (level + 10);
    } else {
        calc += 5;
    }

    pokemon_stat_set(pdata, stat, NONE, calc);
}

void pokemon_stat_memcpy(PokemonData* dst, PokemonData* src, uint8_t which) {
    if (!dst || !src || !dst->trade_block || !src->trade_block) return;
    if (which >= 6) which = 0;

    if (dst->gen == GEN_I) {
        ((TradeBlockGenI*)dst->trade_block)->party_members[0] =
            ((TradeBlockGenI*)src->trade_block)->party_members[which];
        memcpy(&(((TradeBlockGenI*)dst->trade_block)->party[0]),
               &(((TradeBlockGenI*)src->trade_block)->party[which]),
               sizeof(PokemonPartyGenI));
        memcpy(&(((TradeBlockGenI*)dst->trade_block)->nickname[0]),
               &(((TradeBlockGenI*)src->trade_block)->nickname[which]),
               sizeof(struct name));
        memcpy(&(((TradeBlockGenI*)dst->trade_block)->ot_name[0]),
               &(((TradeBlockGenI*)src->trade_block)->ot_name[which]),
               sizeof(struct name));
    } else if (dst->gen == GEN_II) {
        ((TradeBlockGenII*)dst->trade_block)->party_members[0] =
            ((TradeBlockGenII*)src->trade_block)->party_members[which];
        memcpy(&(((TradeBlockGenII*)dst->trade_block)->party[0]),
               &(((TradeBlockGenII*)src->trade_block)->party[which]),
               sizeof(PokemonPartyGenII));
        memcpy(&(((TradeBlockGenII*)dst->trade_block)->nickname[0]),
               &(((TradeBlockGenII*)src->trade_block)->nickname[which]),
               sizeof(struct name));
        memcpy(&(((TradeBlockGenII*)dst->trade_block)->ot_name[0]),
               &(((TradeBlockGenII*)src->trade_block)->ot_name[which]),
               sizeof(struct name));
    }
}

bool pokemon_is_shiny(PokemonData* pdata) {
    if (!pdata || pdata->gen != GEN_II) return false;
    uint8_t atk_iv = pokemon_stat_get(pdata, STAT_ATK_IV, NONE);
    uint8_t def_iv = pokemon_stat_get(pdata, STAT_DEF_IV, NONE);
    uint8_t spd_iv = pokemon_stat_get(pdata, STAT_SPD_IV, NONE);
    uint8_t spc_iv = pokemon_stat_get(pdata, STAT_SPC_IV, NONE);

    if (spd_iv != 10 || def_iv != 10 || spc_iv != 10) return false;
    return (atk_iv == 2 || atk_iv == 3 || atk_iv == 6 || atk_iv == 7 ||
            atk_iv == 10 || atk_iv == 11 || atk_iv == 14 || atk_iv == 15);
}

void pokemon_set_shiny(PokemonData* pdata, bool shiny) {
    if (!pdata || pdata->gen != GEN_II) return;
    if (shiny) {
        pokemon_stat_set(pdata, STAT_DEF_IV, NONE, 10);
        pokemon_stat_set(pdata, STAT_SPD_IV, NONE, 10);
        pokemon_stat_set(pdata, STAT_SPC_IV, NONE, 10);
        pokemon_stat_set(pdata, STAT_ATK_IV, NONE, 10);
        for (int i = STAT; i < STAT_END; i++) {
            pokemon_stat_calc(pdata, (DataStat)i);
        }
    } else if (pokemon_is_shiny(pdata)) {
        // If it was shiny, restore IVs according to selected preset
        pokemon_stat_iv_calc(pdata, pdata->stat_sel);
        if (pokemon_is_shiny(pdata)) {
            pokemon_stat_set(pdata, STAT_DEF_IV, NONE, 15);
        }
        for (int i = STAT; i < STAT_END; i++) {
            pokemon_stat_calc(pdata, (DataStat)i);
        }
    }
}
