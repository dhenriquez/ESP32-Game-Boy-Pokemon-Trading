#!/usr/bin/env python3
"""
generate_sprites.py
Syncs monochrome Pokémon sprites from Flipper-Zero-Game-Boy-Pokemon-Trading/sprites
into both the web preview directory (html/sprites/) and the ESP32 PROGMEM
firmware source (src/pokemon_sprites.cpp and include/pokemon_sprites.h).
"""

import os
import shutil
import glob

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FLIPPER_SPRITES_DIR = os.path.join(REPO_ROOT, "Flipper-Zero-Game-Boy-Pokemon-Trading", "sprites")
HTML_SPRITES_DIR = os.path.join(REPO_ROOT, "html", "sprites")
CPP_OUTPUT_PATH = os.path.join(REPO_ROOT, "src", "pokemon_sprites.cpp")
H_OUTPUT_PATH = os.path.join(REPO_ROOT, "include", "pokemon_sprites.h")

def main():
    if not os.path.exists(FLIPPER_SPRITES_DIR):
        print(f"Error: Sprites directory not found: {FLIPPER_SPRITES_DIR}")
        return

    os.makedirs(HTML_SPRITES_DIR, exist_ok=True)
    os.makedirs(os.path.dirname(CPP_OUTPUT_PATH), exist_ok=True)
    os.makedirs(os.path.dirname(H_OUTPUT_PATH), exist_ok=True)

    sprite_files = sorted(glob.glob(os.path.join(FLIPPER_SPRITES_DIR, "*.png")))
    print(f"Found {len(sprite_files)} sprites in {FLIPPER_SPRITES_DIR}")

    # 1. Copy to html/sprites/
    copied = 0
    for src in sprite_files:
        dst = os.path.join(HTML_SPRITES_DIR, os.path.basename(src))
        shutil.copy2(src, dst)
        copied += 1
    print(f"Copied {copied} sprites to {HTML_SPRITES_DIR}")

    # 2. Generate include/pokemon_sprites.h
    with open(H_OUTPUT_PATH, "w", encoding="utf-8") as h_file:
        h_file.write("""#pragma once

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
""")
    print(f"Generated header: {H_OUTPUT_PATH}")

    # 3. Generate src/pokemon_sprites.cpp
    total_bytes = 0
    with open(CPP_OUTPUT_PATH, "w", encoding="utf-8") as cpp_file:
        cpp_file.write("""#include "pokemon_sprites.h"
#include <pgmspace.h>

""")

        table_entries = []
        for i in range(252):
            fname = f"{i:03d}.png"
            fpath = os.path.join(FLIPPER_SPRITES_DIR, fname)
            if not os.path.exists(fpath):
                print(f"Warning: {fname} missing, using empty fallback")
                table_entries.append("{ NULL, 0 }")
                continue

            with open(fpath, "rb") as f:
                data = f.read()

            total_bytes += len(data)
            cpp_file.write(f"static const uint8_t sprite_{i:03d}[] PROGMEM = {{\n")
            # Write 16 bytes per line
            for chunk_start in range(0, len(data), 16):
                chunk = data[chunk_start:chunk_start+16]
                line = "  " + ", ".join(f"0x{b:02x}" for b in chunk) + ",\n"
                cpp_file.write(line)
            cpp_file.write("};\n\n")

            table_entries.append(f"  {{ sprite_{i:03d}, {len(data)} }},")

        cpp_file.write("static const EmbeddedSprite sprite_table[252] PROGMEM = {\n")
        for entry in table_entries:
            cpp_file.write(entry + "\n")
        cpp_file.write("};\n\n")

        cpp_file.write("""EmbeddedSprite get_embedded_sprite(uint16_t num) {
    if (num > 251) {
        EmbeddedSprite empty = { NULL, 0 };
        return empty;
    }
    EmbeddedSprite sp;
    #if defined(ESP32)
    sp = sprite_table[num];
    #else
    memcpy_P(&sp, &sprite_table[num], sizeof(EmbeddedSprite));
    #endif
    return sp;
}
""")

    print(f"Generated {CPP_OUTPUT_PATH} ({total_bytes} bytes of sprite payload across 252 Pokémon)")

if __name__ == "__main__":
    main()
