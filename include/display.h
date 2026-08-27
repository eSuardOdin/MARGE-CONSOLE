#ifndef DISPLAY_H
#define DISPLAY_H

#include "./bus.h"
#include "./common.h"
#include "./object.h"
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
void display_map(bus* bus);


/**
 * @brief Puts the objects color indexes in "framebuffer"
 * 
 * @param bus 
 */
void display_objects(bus* bus);

/**
 * @brief Displays a tile on the framebuffer
 * 
 * @param bus 
 * @param x 
 * @param y 
 * @param tile_address 
 * @param is_object 
 */
void display_tile(bus* bus, int x, int y, int tile_address, char is_transparent_enabled, char is_x_flipped, char is_y_flipped);
#endif