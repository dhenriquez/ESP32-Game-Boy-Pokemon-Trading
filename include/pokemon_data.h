#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "named_list.h"
#include "stat_nl.h"
#include "pokemon_table.h"
#include "stats.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Generation defines */
#ifndef GEN_I
#define GEN_I 0x01
#endif
#ifndef GEN_II
#define GEN_II 0x02
#endif

/* Length constants */
#define LEN_NAME_BUF 11
#define LEN_NICKNAME 11
#define LEN_OT_NAME 8
#define LEN_NUM_BUF 6
#define LEN_LEVEL 4
#define LEN_OT_ID 6

typedef struct pokemon_party_data_gen_i PokemonPartyGenI;
typedef struct trade_block_gen_i TradeBlockGenI;
typedef struct pokemon_party_data_gen_ii PokemonPartyGenII;
typedef struct trade_block_gen_ii TradeBlockGenII;

/* 44 bytes in memory */
struct __attribute__((__packed__)) pokemon_party_data_gen_i {
    uint8_t index;
    uint16_t hp;
    uint8_t level;
    uint8_t status_condition;
    uint8_t type[2];
    uint8_t catch_held;
    uint8_t move[4];
    uint16_t ot_id;
    uint8_t exp[3];
    uint16_t hp_ev;
    uint16_t atk_ev;
    uint16_t def_ev;
    uint16_t spd_ev;
    uint16_t spc_ev;
    uint16_t iv;
    uint8_t move_pp[4];
    uint8_t level_again;
    uint16_t max_hp;
    uint16_t atk;
    uint16_t def;
    uint16_t spd;
    uint16_t spc;
};

struct __attribute__((__packed__)) name {
    uint8_t str[LEN_NAME_BUF];
};
typedef struct name Name;

/* 415 bytes */
struct __attribute__((__packed__)) trade_block_gen_i {
    Name trainer_name;
    uint8_t party_cnt;
    uint8_t party_members[7];
    PokemonPartyGenI party[6];
    Name ot_name[6];
    Name nickname[6];
};

/* 48 bytes */
struct __attribute__((__packed__)) pokemon_party_data_gen_ii {
    uint8_t index;
    uint8_t held_item;
    uint8_t move[4];
    uint16_t ot_id;
    uint8_t exp[3];
    uint16_t hp_ev;
    uint16_t atk_ev;
    uint16_t def_ev;
    uint16_t spd_ev;
    uint16_t spc_ev;
    uint16_t iv;
    uint8_t move_pp[4];
    uint8_t friendship;
    uint8_t pokerus;
    uint16_t caught_data;
    uint8_t level;
    uint8_t status_condition;
    uint8_t unused;
    uint16_t hp;
    uint16_t max_hp;
    uint16_t atk;
    uint16_t def;
    uint16_t spd;
    uint16_t spc_atk;
    uint16_t spc_def;
};

/* 441 bytes */
struct __attribute__((__packed__)) trade_block_gen_ii {
    Name trainer_name;
    uint8_t party_cnt;
    uint8_t party_members[7];
    uint16_t trainer_id;
    PokemonPartyGenII party[6];
    Name ot_name[6];
    Name nickname[6];
};

struct pokemon_data {
    const NamedList* move_list;
    const NamedList* stat_list;
    const NamedList* type_list;
    const NamedList* item_list;
    const PokemonTable* pokemon_table;

    void* trade_block;
    size_t trade_block_sz;

    void* party;
    size_t party_sz;

    EvIvPreset stat_sel;
    uint8_t gen;
    uint8_t dex_max;
};
typedef struct pokemon_data PokemonData;

PokemonData* pokemon_data_alloc(uint8_t gen);
void pokemon_data_free(PokemonData* pdata);

void pokemon_stat_memcpy(PokemonData* dst, PokemonData* src, uint8_t which);
uint16_t pokemon_stat_get(PokemonData* pdata, DataStat stat, DataStatSub num);
void pokemon_stat_set(PokemonData* pdata, DataStat stat, DataStatSub which, uint16_t val);
uint16_t pokemon_stat_ev_get(PokemonData* pdata, DataStat stat);
void pokemon_stat_ev_set(PokemonData* pdata, DataStat stat, uint16_t val);
void pokemon_stat_iv_set(PokemonData* pdata, int val);
void pokemon_exp_set(PokemonData* pdata, uint32_t exp);
void pokemon_exp_calc(PokemonData* pdata);
void pokemon_stat_calc(PokemonData* pdata, DataStat stat);
void pokemon_default_nickname_set(char* dest, PokemonData* pdata, size_t n);
void pokemon_name_set(PokemonData* pdata, DataStat stat, const char* name);
void pokemon_name_get(PokemonData* pdata, DataStat stat, char* dest, size_t len);

/* Gender, Shiny & Pokerus helpers */
bool pokemon_is_shiny(PokemonData* pdata);
void pokemon_set_shiny(PokemonData* pdata, bool shiny);

#ifdef __cplusplus
}
#endif
