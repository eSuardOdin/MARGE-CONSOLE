#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "common.h"
#include <stdint.h>

typedef struct cartridge_t
{
    uint8_t     ram[0x20000];
    uint8_t*    rom;
} cartridge_t;

int init_cartridge(cartridge_t* cart, uint8_t* rom);

#endif