#include "cpu.h"
#include "cartridge.h"
#include "bus.h"
#include "common.h"
#include "display.h"
#include "io.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_audio.h>
#include <stdio.h>
#include <stdlib.h>


#define FILE_OFFST  0x2000
#ifndef ROM_SIZE
#define ROM_SIZE    65535
#endif
#ifndef DEBUG
#define DEBUG
#endif

int main(int argc, char** argv)
{

    if(argc != 2)
    {
        fprintf(stderr, "Usage: riscv_emu <executable_filepath>\n");
    }

    // Read instruction and data
    uint8_t* ROM;
    size_t rom_size = get_ptr_to_romdata(argv[1], &ROM);
    
    // Load cartridge
    cartridge_t cartridge;
    init_cartridge(&cartridge, ROM);
    
    // Link to bus
    bus bus;
    init_bus(&bus, &cartridge);
    
    cpu_t cpu;
    init_cpu(&cpu, &bus);

    // Put .data in memory
    for(int i = 0; i < 0x1000 && (RAM_OFST + i) < rom_size; i++)    // 1000 is arbitrary .data size, need a constant
    {                                                               // to be checked at compile time with lib
        cpu.bus->ram[i] = ROM[RAM_OFST + i];
    }
    int res;




    SDL_Renderer *renderer;
	SDL_Window *window;
    SDL_Texture *texture;
    int width   = SCREEN_WIDTH*SCALE;
    int height  = SCREEN_HEIGHT*SCALE;
    int gFrameBuffer[width*height];

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO))
    {
        fprintf(stderr, "Error while initializing SDL : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    // Create Window
    window = SDL_CreateWindow("MARGE", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        width,height, 0);
    if(window == NULL)
	{
		SDL_Quit();
        return EXIT_FAILURE;
	}
	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	if(renderer == NULL)
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 
                                  width, height);



    // State variables
    int is_ebreak = 0;          // Check if any instructions to execute
    int running = 1;            // Is program still running
    SDL_Event event;
    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    handle_key_event(&event, &bus);
                    break;
                default: break;
            }
        }


        // *** Main execution ***
        // Execute instructions while not in 1/60 sec - Full useless
        for (int i = 0; i < INST_PER_FRAME && !is_ebreak; i++)
        {
            fetch_instruction(&cpu, ROM);
            res = decode_execute_instruction(&cpu);
            if(res == 1)    // If EBREAK called ( see cpu.c )
            {
                is_ebreak = 1;
            }
        }

        // *** rendering logic ***  TODO: MOVE IN DISPLAY
        display_map(&bus);
        display_objects(&bus);

        int col; 
        for(int i = 0; i < 240*160; i++)
        {
            // Scale the display
            for(int x = 0; x < SCALE; x++)
            {
                for(int y = 0; y < SCALE; y++)
                {
                    int srcX = i % SCREEN_WIDTH;
                    int srcY = i / SCREEN_WIDTH;
                    int destX = srcX * SCALE;
                    int destY = srcY * SCALE;
                    // pixels[(destY + y) * Width * Scale + (destX+x)] = BGPalette[FrameBuffer[i]];
                    int c = COLORSPAL[cpu.bus->framebuffer[i] & 0x1F];
                    gFrameBuffer[( destY + y) * SCREEN_WIDTH * SCALE + (destX + x)] = (((c & 0xFF0000) << 8) | ((c & 0x00FF00) << 8) | ((c & 0x0000FF) << 8) | 0xFF);
                    //(destY + y) * Width * Scale + (destX+x)
                }
            }
        }

        SDL_UpdateTexture(texture, NULL, gFrameBuffer, width * sizeof(int));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        // Increment frame counter - TODO : REPLACE WITH INTERRUPT
        int current_frame = read_memory(&bus, FRAME_COUNTER);
        write_memory(&bus, current_frame+1, FRAME_COUNTER);
    }


    return 0;
}