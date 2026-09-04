#include "io.h"
#include "bus.h"
#include "common.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>

void handle_key_event(SDL_Event* event, bus_t* bus)
{
    // Check if key is pressed or released
    char is_pressed = (event->type == SDL_KEYDOWN) ? 1 : 0; 
    switch (event->key.keysym.sym )
    {
        case SDLK_LEFT:
            write_joypad(is_pressed, JOYPAD_LEFT, bus, JOYPAD_0);  
            break;
        case SDLK_RIGHT:
            write_joypad(is_pressed, JOYPAD_RIGHT, bus, JOYPAD_0);  
            break;
        case SDLK_UP:
            write_joypad(is_pressed, JOYPAD_UP, bus, JOYPAD_0);  
            break;
        case SDLK_DOWN:
            write_joypad(is_pressed, JOYPAD_DOWN, bus, JOYPAD_0);  
            break;
    }
    
}



void write_joypad(char is_pressed, int value_mask, bus_t* bus, int32_t joypad_address)
{
    uint current_state = read_memory(bus, IO_OFST);
    if(is_pressed)
    {
        // Add pressed key
        current_state |= value_mask;
    }
    else
    {
        // Remove unpressed key
        current_state &= (~value_mask);
    }

    write_memory(bus, current_state, joypad_address);
    // printf("Value of joypad register: %08X\n", current_state);
}
