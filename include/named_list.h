#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct __attribute__((__packed__)) named_list {
    const char* name;
    const uint8_t index;
    const uint8_t gen;
};

typedef struct named_list NamedList;

size_t namedlist_cnt(const NamedList* list);
uint32_t namedlist_gen_get_pos(const NamedList* list, uint32_t pos);
uint32_t namedlist_gen_get_index(const NamedList* list, uint32_t index);
uint32_t namedlist_pos_get(const NamedList* list, uint32_t index);
uint32_t namedlist_index_get(const NamedList* list, uint32_t pos);
const char* namedlist_name_get_index(const NamedList* list, uint32_t index);
const char* namedlist_name_get_pos(const NamedList* list, uint32_t pos);

#ifdef __cplusplus
}
#endif
