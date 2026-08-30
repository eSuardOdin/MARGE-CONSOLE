#include "cartridge.h"


int init_cartridge(cartridge_t* cart, uint8_t* rom, size_t rom_size)
{
    cart->rom = rom;
    cart->rom_size = rom_size;
    return 0;
}