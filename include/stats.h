#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    /* Base stats */
    STAT_BASE = 0,
    STAT_BASE_ATK = 0,
    STAT_BASE_DEF,
    STAT_BASE_SPD,
    STAT_BASE_SPC,
    STAT_BASE_SPC_ATK,
    STAT_BASE_SPC_DEF,
    STAT_BASE_HP,
    STAT_BASE_TYPE,
    STAT_BASE_MOVE,
    STAT_BASE_GROWTH,
    STAT_BASE_GENDER_RATIO,
    STAT_BASE_INDEX,
    STAT_BASE_END,

    /* In-party stats */
    STAT = 0,
    STAT_ATK = 0,
    STAT_DEF,
    STAT_SPD,
    STAT_SPC,
    STAT_SPC_ATK,
    STAT_SPC_DEF,
    STAT_HP,
    STAT_END = 7,
    STAT_TYPE = 7,
    STAT_MOVE,

    STAT_EV = 10,
    STAT_EV_OFFS = 10,
    STAT_ATK_EV = 10,
    STAT_DEF_EV,
    STAT_SPD_EV,
    STAT_SPC_EV,
    STAT_SPC_ATK_EV,
    STAT_SPC_DEF_EV,
    STAT_HP_EV = 16,
    STAT_EV_END,

    STAT_IV = 19,
    STAT_IV_OFFS = 20,
    STAT_ATK_IV = 20,
    STAT_DEF_IV,
    STAT_SPD_IV,
    STAT_SPC_IV,
    STAT_SPC_ATK_IV,
    STAT_SPC_DEF_IV,
    STAT_HP_IV = 26,
    STAT_IV_END,

    /* Other attributes */
    STAT_LEVEL = 28,
    STAT_INDEX,
    STAT_NUM,
    STAT_CONDITION,
    STAT_NICKNAME,
    STAT_OT_NAME,
    STAT_OT_ID,
    STAT_TRAINER_NAME,
    STAT_SEL,
    STAT_EXP,
    STAT_HELD_ITEM,
    STAT_POKERUS,
} DataStat;

typedef enum {
    MOVE_0 = 0,
    MOVE_1,
    MOVE_2,
    MOVE_3,

    TYPE_0 = 0,
    TYPE_1,

    EXP_0 = 0,
    EXP_1,
    EXP_2,

    NONE = 0,
} DataStatSub;

typedef enum {
    EV_IV_ALL_RANDOM,
    EV_IV_MAX_IV,
    EV_IV_MAX_EV,
    EV_IV_MAX_EV_FOR_LEVEL,
    EV_IV_ZERO_EV,
    EV_IV_COUNT,
} EvIv;

#ifdef __cplusplus
}
#endif
