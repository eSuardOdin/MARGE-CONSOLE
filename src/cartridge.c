#include "cartridge.h"


int init_cartridge(cartridge_t* cart, uint8_t* rom)
{
    cart->rom = rom;
    return 0;
}