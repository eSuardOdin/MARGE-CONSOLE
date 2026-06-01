#include "display.h"
#include "bus.h"
#include "common.h"
#include <string.h>



void display_map(bus_t* bus, SDL_Renderer* renderer, SDL_Texture* texture)
{
    // Get current map to print
    int map_index = read_memory(bus, MAP_INDEX);

    // Get map address by adding map length
    uint32_t map_address = MAPS_OFST + (30 * 20 * 2 * map_index);

    int tile_offset;
    int fb_offset;
    // Draw sprites
    for(int x = 0; x < 30; x++)
    {
        for(int y = 0; y < 20; y++)
        {
            int tile_index = read_memory(bus, map_address) | (read_memory(bus, map_address+1) << 8);

            for (int row = 0; row < 8; row++)
            {
                // Get the offset in X + the offset on Y axis multiplied by the number of a width pixel (30 * 8)
                fb_offset = x * 8 + (y * 8 + row) * 30 * 8; 
                tile_offset = tile_index * 64 + row * 8;
                memcpy(bus->framebuffer + fb_offset, bus->tileset + tile_offset, 8);           
            }

            map_address += 2;
        }
    }

}