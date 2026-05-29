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

    int tile[64];
    // Draw sprites
    for (int x = 0; x < 30; x++)
    {
        int tile_index = read_memory(bus, map_address) | (read_memory(bus, map_address+1) << 8); 
        for(int y = 0; y < 20; y++)
        {
            memcpy( bus->framebuffer + x * y * 64, bus->tileset + tile_index, 64 );
        }
    }

}