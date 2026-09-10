#include <stdlib.h>
#include "patch_list.h"

PatchList* plist_alloc(void) {
    PatchList* plist = (PatchList*)malloc(sizeof(PatchList));
    if (!plist) return NULL;
    plist->index = 0;
    plist->next = NULL;
    return plist;
}

void plist_append(PatchList* plist, uint8_t index) {
    if (!plist) return;
    while (plist->next != NULL) {
        plist = plist->next;
    }
    plist->index = index;
    plist->next = plist_alloc();
}

void plist_free(PatchList* plist) {
    PatchList* curr = plist;
    while (curr != NULL) {
        PatchList* next = curr->next;
        free(curr);
        curr = next;
    }
}

uint8_t plist_index_get(PatchList* plist, int offset) {
    if (!plist || offset < 0) return 0;
    PatchList* curr = plist;
    for (int i = 0; i < offset; i++) {
        if (curr->next == NULL) break;
        curr = curr->next;
    }
    return curr->index;
}

void plist_create(PatchList** pplist, PokemonData* pdata) {
    if (!pdata || !pplist) return;
    uint8_t* trade_party_flat = (uint8_t*)pdata->party;

    if (*pplist != NULL) {
        plist_free(*pplist);
        *pplist = NULL;
    }

    *pplist = plist_alloc();

    /*
     * Game Boy patch list mechanism:
     * Part 1 covers offsets 0x00 - 0xFB (expressed as index 0x01 - 0xFC).
     * 0xFF signifies end of part 1.
     * Part 2 covers offsets 0xFC - end.
     * 0xFF signifies end of part 2.
     * If party byte is 0xFE, replace with 0xFF and record (offset % 0xFC) + 1.
     */
    for (size_t i = 0; i < pdata->party_sz; i++) {
        if (i == 0xFC) {
            plist_append(*pplist, 0xFF);
        }

        if (trade_party_flat[i] == 0xFE) {
            plist_append(*pplist, (uint8_t)((i % 0xFC) + 1));
            trade_party_flat[i] = 0xFF;
        }
    }

    plist_append(*pplist, 0xFF);
}
