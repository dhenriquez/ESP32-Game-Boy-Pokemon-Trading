#pragma once

#include "named_list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RANDIV_ZEROEV,
    RANDIV_LEVELEV,
    RANDIV_MAXEV,
    MAXIV_ZEROEV,
    MAXIV_LEVELEV,
    MAXIV_MAXEV,
    EVIV_PRESET_COUNT
} EvIvPreset;

extern const NamedList stat_list[];

#ifdef __cplusplus
}
#endif
