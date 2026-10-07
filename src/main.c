#include "cpu.h"
#include "cartridge.h"
#include "bus.h"
#include "common.h"
#include "display.h"
#include "io.h"
#include "loader.h"
#include "apu.h"
#include "timer.h"

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
        exit(EXIT_FAILURE);
    }


    // Load cartridge
    uint32_t entrypoint = 0;
    FILE* rom_file = get_elf_file(argv[1], &entrypoint);
    load_segments(rom_file);
    cartridge_t cartridge;
    if(load_cartridge(&cartridge, rom_file))
    {
        printf("Error when loading the cartridge\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Cartrige loaded successfully, size : %.8Xb\n", cartridge.rom_size);
    }
    
    // Init APU
    apu_t* apu = init_apu();
    // Init timer
    timer tim;
    init_timer(&tim);
    // Link to bus
    bus_t bus;
    init_bus(&bus, &cartridge, apu, &tim);
    
    cpu_t cpu;
    // Put .data in memory
    if(load_data_in_ram(&bus, rom_file))
    {
        printf("Error when loading the data in RAM\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("data loaded in RAM\n");
    }

    printf("Entrypoint : %.2X\n", entrypoint);
    init_cpu(&cpu, &bus, entrypoint);

    int res;


    SDL_Renderer *renderer;
	SDL_Window *window;
    SDL_Texture *texture;
    int width   = SCREEN_WIDTH*SCALE;
    int height  = SCREEN_HEIGHT*SCALE;
    int gFrameBuffer[width*height];

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
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
    int test_timer = 0;
    int test_sec = 0;
    Uint64 begin_ticks;
    Uint64 end_ticks;
    Uint32 ms_to_wait = 0;
    Uint64 delta = 0;
    SDL_Event event;
    while(running)
    {
        begin_ticks = SDL_GetTicks64();
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
        // Execute instructions while not in 1/60 sec
        for (int i = 0; i < INST_PER_FRAME && !is_ebreak; i++)
        {
            fetch_instruction(&cpu, cartridge.rom);
            res = decode_execute_instruction(&cpu);
            // UPDATE TIMER
            if(!(i % (FREQUENCY_MHZ / TIMER_FREQUENCIES[bus.timer->timer_enable_register & 0x7]))) 
            {
                //printf("Instruction n°%d timer interrupt.\n", i);
                test_timer++;
                if(test_timer >= FREQUENCY_MHZ)
                {
                    test_timer = 0;
                    test_sec++;
                    //write_memory(&bus, bus.ifr | IRQ_TIMER_F, INTERRUPT_FLAGS);
                    printf("[%d]\n", test_sec);
                }
                if(update_timer(bus.timer))
                {
                    write_memory(&bus, bus.ifr | IRQ_TIMER_F, INTERRUPT_FLAGS);
                }
            }
            // UPDATE APU
            if(!(i % (FREQUENCY_MHZ / APU_SWEEP_FREQ)))
            {
                uint8_t deactivate_mask = 0;
                if(!(i % (FREQUENCY_MHZ / APU_FREQUENCY_MHZ)))
                {
                    deactivate_mask |= update_apu_length(apu);
                }
                if(!(i % (FREQUENCY_MHZ / APU_ENV_FREQ)))
                {
                    deactivate_mask |= update_apu_volume(apu); // Nothing desactiveted if vol = 0, Gameboy does it, think it is error prone
                }
                if(update_apu_sweep(apu))
                {
                    fprintf(stderr, "Error while updating sweep.\n");
                    exit(EXIT_FAILURE);
                }
                write_memory(&bus, apu->ar0, AUDIO_GEN_ENABLE);
            }
            if(bus.ime && bus.ifr)
            {
                uint8_t serviced_interrupt = handle_interrupt(&cpu);
                if(!serviced_interrupt) exit(EXIT_FAILURE);
            }
            if(res == 1)    // If EBREAK called ( see cpu.c )
            {
                is_ebreak = 1;
                printf("EBREAK ???? [0x%.8X] - Instruction : [0x%.8X]\n", cpu.pc, cpu.ir);
                exit(EXIT_SUCCESS);
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
                    int c = COLORSPAL[cpu.bus->framebuffer[i] & 0x1F];
                    gFrameBuffer[( destY + y) * SCREEN_WIDTH * SCALE + (destX + x)] = (((c & 0xFF0000) << 8) | ((c & 0x00FF00) << 8) | ((c & 0x0000FF) << 8) | 0xFF);
                }
            }
        }

        SDL_UpdateTexture(texture, NULL, gFrameBuffer, width * sizeof(int));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        write_memory(&bus, bus.ifr | IRQ_FRAME_F, INTERRUPT_FLAGS);
        
        // If frame was quicker than intended, wait or remove time from older delta. 
        end_ticks = SDL_GetTicks64();
        if(end_ticks - begin_ticks < MS_TARGET)
        {
            ms_to_wait = (Uint32)MS_TARGET - ((Uint32)end_ticks - (Uint32)begin_ticks);

            if(delta)
            {
                if(ms_to_wait > delta)
                {
                    ms_to_wait -= delta;
                    delta = 0;
                }
                else
                {
                    delta -= ms_to_wait;
                    ms_to_wait = 0;
                }
            }
            if(ms_to_wait)
            {
                SDL_Delay(ms_to_wait);
            }
        }
        else    // If frame takes longer than intended, add to delta.
        {
            delta += (end_ticks - begin_ticks) - MS_TARGET;
        }

    }


    return 0;
}