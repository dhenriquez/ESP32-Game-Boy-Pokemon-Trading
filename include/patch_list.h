#pragma once
 
#include <stdint.h>
#include <stddef.h>
#include <esp_attr.h>
#include "pokemon_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct patch_list {
    uint8_t index;
    struct patch_list* next;
};
typedef struct patch_list PatchList;

PatchList* plist_alloc(void);
void plist_append(PatchList* plist, uint8_t index);
void plist_free(PatchList* plist);
uint8_t IRAM_ATTR plist_index_get(PatchList* plist, int offset);
void plist_create(PatchList** pplist, PokemonData* pdata);

#ifdef __cplusplus
}
#endif

