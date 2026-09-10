#include "named_list.h"

size_t namedlist_cnt(const NamedList* list) {
    if (!list) return 0;
    size_t i;
    for (i = 0;; i++) {
        if (list[i].name == NULL) return i;
    }
}

uint32_t namedlist_gen_get_pos(const NamedList* list, uint32_t pos) {
    if (!list) return 0;
    return list[pos].gen;
}

uint32_t namedlist_gen_get_index(const NamedList* list, uint32_t index) {
    if (!list) return 0;
    return list[namedlist_pos_get(list, index)].gen;
}

uint32_t namedlist_pos_get(const NamedList* list, uint32_t index) {
    if (!list) return 0;
    for (int i = 0;; i++) {
        if (list[i].name == NULL) break;
        if (index == list[i].index) return i;
    }
    return 0;
}

uint32_t namedlist_index_get(const NamedList* list, uint32_t pos) {
    if (!list) return 0;
    return list[pos].index;
}

const char* namedlist_name_get_index(const NamedList* list, uint32_t index) {
    if (!list) return "";
    return list[namedlist_pos_get(list, index)].name;
}

const char* namedlist_name_get_pos(const NamedList* list, uint32_t pos) {
    if (!list) return "";
    return list[pos].name;
}
