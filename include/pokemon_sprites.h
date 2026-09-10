#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct EmbeddedSprite {
    const uint8_t* data;
    size_t size;
};

EmbeddedSprite get_embedded_sprite(uint16_t num);

#ifdef __cplusplus
}
#endif
