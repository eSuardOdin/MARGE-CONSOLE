#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "common.h"
#include <stdint.h>

typedef struct
{
    uint8_t     ram[0x20000];
    uint8_t*    rom;
    int         rom_size;
} cartridge_t;

int init_cartridge(cartridge_t* cart, uint8_t* rom, size_t rom_size);

#endif