#ifndef IO_H
#define IO_H
#include "common.h"
#include "bus.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_audio.h>


/**
 * @brief Changes the state of the controllers "registers" if needed.
 * 
 * May be called in SLD_PollEvent loop when made sure that event is either
 * keyup or keydown
 * @param event Pointer to the event 
 */
void handle_key_event(SDL_Event* event, bus_t* bus);

/**
 * @brief 
 * 
 * @param is_pressed 
 * @param value_mask 
 * @param bus 
 * @param joypad_address 
 */
void write_joypad(char is_pressed, int value_mask, bus_t *bus, int32_t joypad_address);
#endif