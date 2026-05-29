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
    bus_t bus;
    init_bus(&bus, &cartridge);
    
    cpu_t cpu;
    init_cpu(&cpu, &bus);

    // Put .data in memory
    for(int i = 0; i < 0x1000 && (RAM_OFST + i) < rom_size; i++)    // 1000 is arbitrary .data size, need a constant
    {                                                               // to be checked at compile time with lib
        cpu.bus->ram[i] = ROM[RAM_OFST + i];
    }
    int res;


    // // Dump memory
    // for(int i = 0x404B000; i < 0x404B020; i += 4)
    // {
    //     printf("[0x%08X] %02X %02X %02X %02X\n",
    //         i,
    //         read_memory(cpu.bus, i),
    //         read_memory(cpu.bus, i+1),
    //         read_memory(cpu.bus, i+2),
    //         read_memory(cpu.bus, i+3));
    // }



    // while(1)
    // {
    //     fetch_instruction(&cpu, ROM);
    //     res = decode_execute_instruction(&cpu);
    //     if(res == 1)    // If EBREAK called ( see cpu.c )
    //     {
    //         break;
    //     }
    // }
    // printf("ENDED EXECUTION\n");
    // // Dump memory
    // for(int i = 0x404B000; i < 0x404B020; i += 4)
    // {
    //     printf("[0x%08X] %02X %02X %02X %02X\n",
    //         i,
    //         read_memory(cpu.bus, i),
    //         read_memory(cpu.bus, i+1),
    //         read_memory(cpu.bus, i+2),
    //         read_memory(cpu.bus, i+3));
    // }




    SDL_Renderer *renderer;
	SDL_Window *window;
    SDL_Texture *texture;
    int gFrameBuffer[240*160];
    int width   = 240;
    int height  = 160;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO))
        return EXIT_FAILURE;
    // Create Window
    window = SDL_CreateWindow("CPU Display test", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        width, height, 0);
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



    // Frame calculation variables
    Uint64 sdl_start;
    Uint64 sdl_end;     

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
        sdl_start = SDL_GetTicks64();
        //sdl_end = sdl_start + sdl_delta;
        // Execute instructions while not in 1/60 sec
        for (int i = 0; i < INST_PER_FRAME && !is_ebreak; i++)
        {
            fetch_instruction(&cpu, ROM);
            res = decode_execute_instruction(&cpu);
            if(res == 1)    // If EBREAK called ( see cpu.c )
            {
                is_ebreak = 1;
            }
        }
        //if (SDL_GetTicks64() - (sdl_start + sdl_delta) <= 16.66667) break;
        

        // *** rendering logic ***
        display_map(&bus, renderer, texture);
        int col; 
        for(int i = 0; i < 240*160; i++)
        {
            col = (COLORSPAL[cpu.bus->framebuffer[i] & 0x1F] << 8) | 0xFF;
            gFrameBuffer[i] = col;
        }
        char* pix;
        int pitch;
    
        //printf("fb[0] = %d\n", cpu.bus->framebuffer[0]);

        SDL_UpdateTexture(texture, NULL, gFrameBuffer, width * sizeof(int));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        


        // Wait in order to get to 60 FPS
        // int elapsed_ms = SDL_GetTicks64() - sdl_start;
        //if(elapsed_ms < 1000 / FPS_TARGET)
        // {
        //     SDL_Delay(1000 / FPS_TARGET - elapsed_ms);
        // }
        // int elapsed_ms = SDL_GetTicks64() - sdl_start;
        // int to_wait = 1000/FPS_TARGET - elapsed_ms;
        // if(to_wait > 0)
        // {
        //     SDL_Delay((uint32_t) (1000/FPS_TARGET - elapsed_ms));
        // }
    }


    dump_memory(cpu.bus, 0, 0x100);
    return 0;
}