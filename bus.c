#include "bus.h"
#include "cartridge.h"
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


void write_memory(bus_t* bus, uint8_t data, int32_t addr)
{
    if(addr < VRAM_OFST)
    {
        if(addr < ROM_OFST)
        {
            bus->cartridge->ram[addr] = data;
            
            printf("WRITTEN: [%08X]: %08X\n", addr, data);
            
        }
    }
}
