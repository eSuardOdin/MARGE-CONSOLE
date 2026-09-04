#include "bus.h"
#include "cartridge.h"
#include "common.h"
#include "object.h"


int init_bus(bus* bus, cartridge_t* cart)
{
    bus->cartridge = cart;
    // Init framebuffer
    for(int i = 0; i < 0x9600; i++)
    {
        bus->framebuffer[i] = 0;
    }
    // Init controller
    bus->controller = 0;
    // Init map index
    bus->map_index = 0;

    // Init scroll registers
    bus->scroll_x = bus->scroll_y = 0;

    // Init frame counter
    bus->frame_counter = 0;

    // Init OAM
    for(int i = 0; i < OBJECT_NUMBER * sizeof(object_t); i++)
    {
        bus->oam[i] = 0;
    }
    return 0;
}


uint8_t read_memory(bus* bus, int32_t addr)
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
        else if(addr == MAP_INDEX)
        {
            // printf("RD map index address [%08X], got [%02X]\n", addr, bus->map_index);
            return bus->map_index;
        }
        // Get scroll X register
        else if(addr == SCROLL_X)
        {
            return bus->scroll_x;
        }
        // Get scroll Y register
        else if(addr == SCROLL_Y)
        {
            return bus->scroll_y;
        }
        // Get current frame
        else if(addr == FRAME_COUNTER)
        {
            return bus->frame_counter;
        }


    }

    // If reading from tileset memory
    else if (addr >= TILESET_OFST && addr < MAPS_OFST)
    {
        return bus->tileset[addr - TILESET_OFST];
    }

    // If reading from tilemap memory
    else if (addr >= MAPS_OFST && addr < OAM_OFST)
    {
        return bus->maps[addr - MAPS_OFST];
    }

    // Reading from OAM memory
    else if(addr >= OAM_OFST && addr < AUDIO_OFST)
    {
        return bus->oam[addr - OAM_OFST];
    }

    else if(addr >= AUDIO_OFST && addr < STACK_OFST)
    {
        printf("RD from audio registers\n");
        return 0xFF;
    }

    else if(addr >= STACK_OFST && addr <= STACK_END)
    {
        return bus->stack[addr - STACK_OFST];
    }
    
    // If data could not be retreived, send garbage.
    return 0xFF;
}


void write_memory(bus* bus, uint8_t data, int32_t addr)
{
    //printf("WR to [%08X] - DATA : [%02X]\n", addr, data);
    
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
        printf("Write on framebuffer: [%08X]: %08X\n", addr, data);
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
            // printf("WT map index - [%02X]\n", data);
            bus->map_index = data;
        }
        // Set scroll X register
        else if(addr == SCROLL_X)
        {
            bus->scroll_x = data;
        }
        // Set scroll Y register
        else if(addr == SCROLL_Y)
        {
            bus->scroll_y = data;
        }
        else if(addr == FRAME_COUNTER)
        {
            bus->frame_counter = data;
        }
    }

    // If writing in tileset memory
    else if (addr >= TILESET_OFST && addr < MAPS_OFST)
    {
        bus->tileset[addr - TILESET_OFST] = data;
    }

    // If writing in tilemap memory
    else if (addr >= MAPS_OFST && addr < OAM_OFST)
    {
        bus->maps[addr - MAPS_OFST] = data;
    }
    // Writing in OAM memory
    else if(addr >= OAM_OFST && addr < AUDIO_OFST)
    {
        bus->oam[addr - OAM_OFST] = data;
    }

    else if(addr >= AUDIO_OFST && addr < STACK_OFST)
    {
        printf("WT in audio registers\n");
    }

    else if(addr >= STACK_OFST && addr <= STACK_END)
    {
        bus->stack[addr - STACK_OFST] = data;
    }

}




// int dump_memory(bus* bus, int start, int size)
// {
//     for(int i = start; i < start + size; i += 4)
//     {
//         printf("[0x%08X] %02X %02X %02X %02X\n",
//             i,
//             read_memory(bus, i),
//             read_memory(bus, i+1),
//             read_memory(bus, i+2),
//             read_memory(bus, i+3));
//     }

//     return 0;
// }