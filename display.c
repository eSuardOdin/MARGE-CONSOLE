#include "display.h"
#include "bus.h"
#include "common.h"
#include <string.h>



void display_map(bus_t* bus, SDL_Renderer* renderer, SDL_Texture* texture)
{
    int scroll_y = read_memory(bus, SCROLL_Y);
    int scroll_x = read_memory(bus, SCROLL_X);
    // Get current map to print
    int map_index = read_memory(bus, MAP_INDEX);

    // Get map address by adding map length
    uint32_t map_address = MAPS_OFST + (64 * 64 * 2 * map_index);

    int tile_offset;
    int fb_offset;

    // Draw sprites
    for(int x = 0; x < 240; x++)
    {
        for(int y = 0; y < 160; y++)
        {
            int posX = (scroll_x + x) % (64 * 8);
            int posY = (scroll_y + y) % (64 * 8);

            int tile_x = posX / 8;
            int tile_y = posY / 8;

            int pix_in_tile_x = posX % 8;
            int pix_in_tile_y = posY % 8;

            int tile_index = read_memory(bus, map_address + (tile_y * 64 + tile_x) * 2)
                        | (read_memory(bus, map_address + (tile_y * 64 + tile_x) * 2 + 1) << 8);

            bus->framebuffer[x + y * 240] = bus->tileset[tile_index * 64 + pix_in_tile_y * 8 + pix_in_tile_x];
        }
    }
}


void display_objects(bus_t* bus, SDL_Renderer* renderer, SDL_Texture* texture)
{
    
}