#include "cpu.h"
#include "cartridge.h"
#include "bus.h"
#include "common.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_audio.h>
#include <stdlib.h>


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
    // Open executable
    FILE* stream = fopen(argv[1], "rb");
    if(stream == NULL)
    {
        perror("open");
    }
    // Get ROM size
    fseek(stream, 0L, SEEK_END);
    long rom_size = ftell(stream);
    printf("ROM SIZE is %ld\n", rom_size);
    if(rom_size > ROM_SIZE)
    {
        fprintf(stderr, "ROM is too large (%ld bytes), %d bytes maximum.\n", rom_size, ROM_SIZE);
    }
    rewind(stream);


    uint8_t* ROM = malloc(rom_size * sizeof(uint8_t));

    // Read ROM
    ssize_t byte_read = fread(ROM, sizeof(uint8_t), rom_size, stream);
    if(byte_read != rom_size)
    {
        fprintf(stderr, "ROM is not loaded.\n");
        exit(EXIT_FAILURE);
    }




    // Load cartridge
    cartridge_t cartridge;
    init_cartridge(&cartridge, ROM);
    
    // Link to bus
    bus_t bus;
    init_bus(&bus, &cartridge);

    cpu_t cpu;
    init_cpu(&cpu, &bus);
    // while(cpu.pc + 4 < byte_read)
    // while(cpu.pc + 4 < 0x2410 * 4)
    while(cpu.pc + 4 < 64)
    {
        fetch_instruction(&cpu, ROM);
        decode_execute_instruction(&cpu);
        printf("\n");
    }
    printf("ENDED EXECUTION\n");



    // Display the framebuffer once to test

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
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, 
                                  width, height);



    int running = 1;
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
                default: break;
            }
        }
        int col; 
        for(int i = 0; i < 240*160; i++)
        {
            col = COLORSPAL[cpu.bus->framebuffer[i]];
            gFrameBuffer[i] = col;
        }
        char* pix;
        int pitch;
    
        SDL_LockTexture(texture, NULL, (void**)&pix, &pitch);
        for (int i = 0, sp = 0, dp = 0; i < height; i++, dp += width, sp += pitch)
            memcpy(pix + sp, gFrameBuffer + dp, width * 4);

        SDL_UnlockTexture(texture);  
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }


    return 0;
}