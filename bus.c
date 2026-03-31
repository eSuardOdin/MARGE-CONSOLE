#include "bus.h"
#include "cartridge.h"
#include "common.h"


int init_bus(bus_t* bus, cartridge_t* cart)
{
    bus->cartridge = cart;
    // Init framebuffer
    for(int i = 0; i < 0x4B000; i++)
    {
        bus->framebuffer[i] = 0;
    }
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
    printf("In write_memory, address is %08X\n", addr);
    if(addr < VRAM_OFST)        // CARTRIDGE
    {
        if(addr < RAM_OFST)
        {
            fprintf(stderr, "Trying to write in ROM cartridge memory (address is %08X).\n", addr);
            exit(EXIT_FAILURE);    
        }

        bus->cartridge->ram[addr - RAM_OFST] = data;
        printf("Write on cartridge RAM: [%08X]: %08X\n", addr, data);
    }
    else if(addr < IO_OFST)     // VRAM
    {
        bus->framebuffer[addr - FB_OFST] = data;
        printf("Write on framebuffer: [%08X]: %08X\n", addr, data);
    }
}
