#ifndef DISPLAY_H
#define DISPLAY_H

#include "bus.h"
#include "common.h"
#include "object.h"
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_audio.h>


/**
 * @brief Puts the background color indexes in "framebuffer"
 * 
 * @param bus 
 */
void display_map(bus_t* bus);


/**
 * @brief Puts the objects color indexes in "framebuffer"
 * 
 * @param bus 
 */
void display_objects(bus_t* bus);

void display_tile(bus_t* bus, int x, int y, int tile_address);
#endif