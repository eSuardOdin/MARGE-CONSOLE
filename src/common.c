#include "common.h"
#include "bus.h"

const int COLORSPAL[32] =
{
    0x000000, 0x00021C, 0x1C284D, 0x343473, 0x2D5280,
    0x4D7A99, 0x7497A6, 0xA3CCD9, 0xF0EDD8, 0x732866,
    0xA6216E, 0xD94C87, 0xD9214F, 0xF25565, 0xF27961,
    0x993649, 0xB36159, 0xF09C60, 0xB38F24, 0xB3B324,
    0xF7C93E, 0x17735F, 0x119955, 0x67B31B, 0x1BA683,
    0x47CCA9, 0x96E3C9, 0x2469B3, 0x0B8BE6, 0x0BAFE6,
    0xF28D85, 0xF0BB90
};


int get_ptr_to_romdata(char *path, uint8_t **rom)
{
    // Open executable
    FILE* stream = fopen(path, "rb");
    if(stream == NULL)
    {
        perror("open");
    }
    // Get ROM size
    fseek(stream, 0L, SEEK_END);
    long rom_size = ftell(stream);
    printf("ROM SIZE is %ld\n", rom_size);
    // if(rom_size > ROM_SIZE)
    // {
    //     fprintf(stderr, "ROM is too large (%ld bytes), %d bytes maximum.\n", rom_size, ROM_SIZE);
    // }
    rewind(stream);


    *rom = malloc(rom_size * sizeof(uint8_t));

    // Read ROM
    ssize_t byte_read = fread(*rom, sizeof(uint8_t), rom_size, stream);
    if(byte_read != rom_size)
    {
        fprintf(stderr, "ROM is not loaded.\n");
        exit(EXIT_FAILURE);
    }
    return rom_size;
}

