#include "bus.h"
#include "common.h"


int init_bus(bus_t* bus, cartridge_t* cart)
{
    bus->cartridge = cart;
    return 0;
}


uint8_t read_memory(bus_t* bus, int32_t addr)
{
    if(addr < VRAM_OFST)
    {

    }

    // If data could not be retreived, send garbage.
    return 0xFF;
}