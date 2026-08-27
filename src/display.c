#include "display.h"
#include "bus.h"
#include "common.h"
#include "object.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void display_map(bus* bus)
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
            int posX = (scroll_x + x) % (TILE_SIZE * 8);
            int posY = (scroll_y + y) % (TILE_SIZE * 8);

            int tile_x = posX / 8;
            int tile_y = posY / 8;

            int pix_in_tile_x = posX % 8;
            int pix_in_tile_y = posY % 8;

            int tile_index = read_memory(bus, map_address + (tile_y * TILE_SIZE + tile_x) * 2)
                        | (read_memory(bus, map_address + (tile_y * TILE_SIZE + tile_x) * 2 + 1) << 8);

            bus->framebuffer[x + y * 240] = bus->tileset[tile_index * TILE_SIZE + pix_in_tile_y * 8 + pix_in_tile_x];
        }
    }
}


void display_objects(bus* bus)
{
    for(int object = 0; object < OBJECT_NUMBER; object++)
    {
        // Copy the struct object from OAM
        object_t* obj = malloc(sizeof(object_t)); 
        if(!obj)
        {
            fprintf(stderr, "display_objects(): Could not allocate memory for object.\n");
            exit(EXIT_FAILURE);
        }
        
        memcpy(obj, bus->oam + object*sizeof(object_t), sizeof(object_t));


        int posX = obj->x_pos - OBJ_SCREEN_OFFSET;
        int posY = obj->y_pos - OBJ_SCREEN_OFFSET;
        
        // Get the object size
        int width = 0;
        int height = 0;
        e_object_resolution res = get_object_resolution(obj->flags, &width, &height);
        
        // Get the current animation tile
        char current_anim_tile = get_object_frame(obj->flags);
        int tile_full_size = TILE_SIZE * width * height;

        // Get tile address in memory, the animation offset (mul by size of total sprite) + index of the first tile
        int tile_address = (current_anim_tile * tile_full_size) + (obj->tile_index * TILE_SIZE);
        
        
        // Check if a flip flag is set on the object to display
        char is_x_flipped = get_object_flip(obj->flags,TRUE);
        char x_tile;
        char is_y_flipped = get_object_flip(obj->flags, FALSE);
        char y_tile;

        // Iterate on Y axis (tiles stored on X axis)
        for (int y = 0; y < (1 << (height-1)); y++)
        {

            // Begin on first or last Y tile depending on Y axis flip value
            y_tile = is_y_flipped ? (1 << (height-1)) - (y+1) : y;
            
            // Check if current tile appears on Y axis (too high)
            if((posY + 8) + y_tile * 8 < 0) continue;
            // Check if current tile is past screen Y (too low) - no need to continue
            if(posY >= SCREEN_HEIGHT) break;
            for (int x = 0; x < (1 << (width-1)); x++)
            {
                x_tile = is_x_flipped ? (1 << (width-1)) - (x+1) : x;
                // Check if current tile appears on X axis (On left)
                if((posX + 8) + x_tile * 8 < 0) continue;
                // Check if current tile is past screen X (On right) - no need to continue
                if(posX >= SCREEN_WIDTH) break;


                // Display the 8x8 tile
                display_tile(
                    bus, 
                    posX + (8 * x), 
                    posY + (8 * y),
                    (tile_address + (y_tile * TILE_SIZE * width) + (x_tile * TILE_SIZE)),
                    is_transparency_enabled(obj->flags),
                    is_x_flipped,
                    is_y_flipped
                );


            }


        }
        // printf("Object n°%d:\n\
        //     X: %d\n\
        //     Y: %d\n\
        //     Tile index: %d\n\
        //     Animation sprites: %d\n\
        //     Resolution: %dx%d\n\
        //     Flags: %d\n\n", object, obj->x_pos, obj->y_pos, obj->tile_index, obj->animation_sprites, width, height, obj->flags);
        free(obj);

    }
}


void display_tile(bus* bus, int x, int y, int tile_address, char is_transparent_enabled, char is_x_flipped, char is_y_flipped)
{
    char tile_y;
    char tile_x;
    
    for (int yy = 0; yy < 8; yy++)
    {
        // If tile is Y flipped, begin on lower pixel.
        tile_y = is_y_flipped ? 7 - yy : yy;
        
        // Do not display if out of screen
        if(y + tile_y < 0) continue;
        if(y + tile_y >= SCREEN_HEIGHT) break;

        for(int xx = 0; xx < 8; xx++)
        {
            // If tile is X flipped, begin on rightmost pixel
            tile_x = is_x_flipped ? 7 - xx : xx;
            if(x + tile_x < 0) continue;
            if(x + tile_x >= SCREEN_WIDTH) break;
            uint8_t color = bus->tileset[tile_address + tile_y * 8 + tile_x];
            //printf("(X: %d, Y: %d) -> Color: %02X\n", tile_x, tile_y)
            if(!(is_transparent_enabled && color == 0))
            {
                bus->framebuffer[(x + xx) + ((y + yy) * 240)] = color;
            }
        }
        // printf("\n");
    }
    //exit(EXIT_SUCCESS);
}