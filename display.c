#include "display.h"
#include "bus.h"
#include "common.h"
#include "object.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void display_map(bus_t* bus)
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


void display_objects(bus_t* bus)
{
    for(int object = 0; object < OBJECT_NUMBER; object++)
    {
        // if(object) break;
        if(object == 9) break;

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
        

        char current_anim_tile = get_object_frame(obj->flags);
        int tile_full_size = TILE_SIZE * width * height;
        // Get tile address in memory, the animation offset (mul by size of total sprite) + index of the first tile
        int tile_address = (current_anim_tile * tile_full_size) + (obj->tile_index * TILE_SIZE);
        
        // --- Can make it better by checking the visibility before the loops --

        // Iterate on Y axis (tiles stored on X axis)
        for (int y = 0; y < (1 << (height-1)); y++)
        {
            // Check if current tile appears on Y axis (too high)
            if((posY + 8) + y * 8 < 0) {/*printf("Sprite of object %d too UP, CONTINUE.\n", object);*/ continue; }
            // Check if current tile is past screen Y (too low) - no need to continue
            if(posY >= SCREEN_HEIGHT) {/*printf("Sprite of object %d too LOW, BREAK.\n", object);*/ break; }
            for (int x = 0; x < (1 << (width-1)); x++)
            {
                // Check if current tile appears on X axis (On left)
                if((posX + 8) + x * 8 < 0) {/*printf("Sprite of object %d too LEFT, CONTINUE.\n", object); */continue; }
                // Check if current tile is past screen X (On right) - no need to continue
                if(posX >= SCREEN_WIDTH) {/*printf("Sprite of object %d too RIGHT, BREAK.\n", object); */break; }


                // Displayable
                display_tile(
                    bus, 
                    posX + (8 * x), 
                    posY + (8 * y),
                    (tile_address + (y * TILE_SIZE * width) + (x * TILE_SIZE)),
                    is_transparency_enabled(obj->flags)
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


void display_tile(bus_t* bus, int x, int y, int tile_address, char is_transparent_enabled)
{
    // printf("Tile located on (%d, %d)\n", x, y);
    for (int tile_y = 0; tile_y < 8; tile_y++)
    {
        // printf(" Y: %03d   ", y + tile_y);
        if(y + tile_y < 0) continue;
        if(y + tile_y >= SCREEN_HEIGHT) break;
        for(int tile_x = 0; tile_x < 8; tile_x++)
        {
            if(x + tile_x < 0) continue;
            if(x + tile_x >= SCREEN_WIDTH) break;
            uint8_t color = bus->tileset[tile_address + tile_y * 8 + tile_x];
            
            if(!(is_transparent_enabled && color == 0))
            {
                bus->framebuffer[(x + tile_x) + ((y + tile_y) * 240)] = color;
            }
        }
        // printf("\n");
    }
}