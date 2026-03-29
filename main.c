#include "cpu.h"
#include "cartridge.h"
#include "bus.h"
#include "common.h"
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
    while(cpu.pc + 4 < 64)
    {
        fetch_instruction(&cpu, ROM);
        decode_execute_instruction(&cpu);
        printf("\n");
    }
    printf("ENDED EXECUTION\n");

    return 0;
}