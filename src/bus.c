#include "bus.h"
#include "timer.h"
#include "apu.h"
#include "cartridge.h"
#include "common.h"
#include "object.h"
#include "debug.h"
#include <stdatomic.h>

int init_bus(bus_t* bus, cartridge_t* cart, apu_t* apu, timer* t)
{
    bus->cartridge = cart;
    bus->apu = apu;
    bus->timer = t;
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

    bus->dbg_register = 0;

    // Init OAM
    for(int i = 0; i < OBJECT_NUMBER * sizeof(object_t); i++)
    {
        bus->oam[i] = 0;
    }
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

    else if(addr >= DEBUG_REG && addr < DEBUG_REG_END)
    {
        return bus->dbg_register;
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
        // TIMERS
        else if(addr == TIMER_ENABLE)
        {
            return bus->timer->timer_enable_register;
        }
        else if(addr == TIMER)
        {
            return bus->timer->timer;
        }
        else if(addr == TIMER_MOD)
        {
            return bus->timer->timer_mod;
        }
        else if(addr == DIV_COUNTER)
        {
            return bus->timer->div_counter;
        }
        else if(addr == INTERRUPT_REGISTER)
        {
            return bus->ime;
        }
        else if(addr == INTERRUPT_FLAGS)
        {
            return bus->ifr;
        }
        else if(addr == DEBUG_REGISTER)
        {
            return bus->dbg_register;
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
        // General APU registers
        if(addr == AUDIO_GEN_ENABLE)
        {
            return bus->apu->ar0;
        }
        else if(addr == AUDIO_GEN_VOLUME)
        {
            return bus->apu->ar1;
        }
        // Channel 0 - TODO : think about needs to read audio channels registers
        else if(addr == C0R0)
        {
            return atomic_load(&bus->apu->c0->r0);
        }
        else if(addr == C0R1)
        {
            return  atomic_load(&bus->apu->c0->r1);
        }

        // Channel 1
        else if(addr == C1R0)
        {
            return atomic_load(&bus->apu->c1->r0);
        }
        else if(addr == C1R1)
        {
            return  atomic_load(&bus->apu->c1->r1);
        }

        // Channel 2
        else if(addr == C2R0)
        {
            return atomic_load(&bus->apu->c2->r0);
        }
        else if(addr == C2R1)
        {
            return  atomic_load(&bus->apu->c2->r1);
        }
    }

    else if(addr >= STACK_OFST && addr <= STACK_END)
    {
        return bus->stack[addr - STACK_OFST];
    } else if (addr == DEBUG_REG) {
        return bus->dbg_register;
    }
    
    // If data could not be retreived, send garbage.
    return 0xFF;
}


void write_memory(bus_t* bus, uint8_t data, int32_t addr)
{
    // printf("WR to [%08X] - DATA : [%02X]\n", addr, data);
    
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
        // TIMERS
        else if(addr == TIMER_ENABLE)
        {
            bus->timer->timer_enable_register = data;
        }
        else if(addr == TIMER)
        {
            // Writing to timer resets it to mod value, regardless of data sent
            bus->timer->timer = bus->timer->timer_mod;
        }
        else if(addr == TIMER_MOD)
        {
            bus->timer->timer_mod = data;
        }
        else if(addr == DIV_COUNTER)
        {
            // Just resets the counter
            bus->timer->div_counter = 0;
        }
        else if(addr == INTERRUPT_REGISTER)
        {
            bus->ime = data;
        }
        else if(addr == INTERRUPT_FLAGS)
        {
            bus->ifr = data;
        }
    }

    else if(addr >= DEBUG_REG && addr <= DEBUG_REG_END)
    {
        print_debug_register(data);
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
        // General APU registers 
        if(addr == AUDIO_GEN_ENABLE)
        {
            atomic_store(&bus->apu->ar0, data);
            apu_set_register_enable(data);
        }
        else if(addr == AUDIO_GEN_VOLUME)
        {
            atomic_store(&bus->apu->ar1, data);
        }
        // Channel 0
        else if(addr == C0R0)
        {
            atomic_store(&bus->apu->c0->r0, data);
            apu_set_channel_freq(0);
        }
        else if(addr == C0R1)
        {
            atomic_store(&bus->apu->c0->r1, data);
            apu_set_channel_freq(0);
        }
        else if(addr == C0R2)
        {
            atomic_store(&bus->apu->c0->length_r, data);
            // printf("LENGTH [0x%.2X]\n", data);
        }
        else if(addr == C0R3)
        {
            atomic_store(&bus->apu->c0->sweep_r, data);
        }
        else if(addr == C0R4)
        {
            atomic_store(&bus->apu->c0->flag_r, data);
        }
        else if(addr == C0R5)
        {
            atomic_store(&bus->apu->c0->volume_r, data);
            // printf("WRITE VOLUME [0x%.2X]\n", data);
            apu_set_channel_volume(0);
        }
    }

    else if(addr >= STACK_OFST && addr <= STACK_END)
    {
        bus->stack[addr - STACK_OFST] = data;
    }

}
