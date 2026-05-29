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
    // Init controller
    bus->controller = 0;
    // Init map index
    bus->map_index = 0;
    return 0;
}


uint8_t read_memory(bus_t* bus, int32_t addr)
{
    // Reading from cartridge
    if(addr < VRAM_OFST)
    {
        if(addr < CART_RAM_OFST)
        {
            return bus->cartridge->rom[addr];
        }
        else
        {
            return bus->cartridge->ram[addr - CART_RAM_OFST];
        }
    }

    // If reading from framebuffer memory ( --- why would I ? --- )
    else if( addr >= VRAM_OFST && addr < RAM_OFST)
    {
        return bus->framebuffer[addr - FB_OFST];
        // printf("Write on framebuffer: [%08X]: %08X\n", addr, data);
    }

    // Reading from in console RAM
    else if (addr >= RAM_OFST && addr < IO_OFST)
    {
        return bus->ram[addr - RAM_OFST];
    }

    // Reading from IO and system registers
    else if (addr >= IO_OFST && addr < TILESET_OFST)
    {
        // Get controller status
        if(addr == JOYPAD_0)
        {
            return bus->controller;
        }
        // Get map index
        if(addr == MAP_INDEX)
        {
            return bus->map_index;
        }
    }

    // If reading from tileset memory
    else if (addr >= TILESET_OFST && addr < MAPS_OFST)
    {
        return bus->tileset[addr - TILESET_OFST];
    }

    // If reading from tilemap memory
    else if (addr >= MAPS_OFST && addr < STACK_OFST)
    {
        return bus->maps[addr - MAPS_OFST];
    }
    
    // If data could not be retreived, send garbage.
    return 0xFF;
}


void write_memory(bus_t* bus, uint8_t data, int32_t addr)
{
    // printf("In write_memory, address is %08X\n", addr);
    
    // If writing in cartridge
    if(addr < VRAM_OFST)
    {
        if(addr < CART_RAM_OFST)
        {
            fprintf(stderr, "Trying to write in ROM cartridge memory (address is %08X).\n", addr);
            exit(EXIT_FAILURE);    
        }

        bus->cartridge->ram[addr - CART_RAM_OFST] = data;
        // printf("Write on cartridge RAM: [%08X]: %08X\n", addr, data);
    }

    // If writing in FRAMEBUFFER
    else if(addr >= FB_OFST && addr < RAM_OFST)
    {
        bus->framebuffer[addr - FB_OFST] = data;
        // printf("Write on framebuffer: [%08X]: %08X\n", addr, data);
    }

    // If writing in RAM
    else if (addr >= RAM_OFST && addr < IO_OFST)
    {
        bus->ram[addr - RAM_OFST] = data;
    }

    // If writing in IO / system registers memory
    else if (addr >= IO_OFST && addr < TILESET_OFST)
    {
        if(addr == JOYPAD_0)
        {
            bus->controller = data;
        }
        // Set map index
        else if(addr == MAP_INDEX)
        {
            bus->map_index = data;
        }
    }

    // If writing in tileset memory
    else if (addr >= TILESET_OFST && addr < MAPS_OFST)
    {
        bus->tileset[addr - TILESET_OFST] = data;
    }

    // If writing in tilemap memory
    else if (addr >= MAPS_OFST && addr < STACK_OFST)
    {
        bus->maps[addr - MAPS_OFST] = data;
    }

}




int dump_memory(bus_t* bus, int start, int size)
{
    for(int i = start; i < start + size; i += 4)
    {
        printf("[0x%08X] %02X %02X %02X %02X\n",
            i,
            read_memory(bus, i),
            read_memory(bus, i+1),
            read_memory(bus, i+2),
            read_memory(bus, i+3));
    }

    return 0;
}