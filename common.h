
#ifndef COMMON_H
#define COMMON_H


#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>


// --- CPU SPECS ---
#define FREQUENCY_MHZ       16800000
#define INSTRUCTION_COST    4
#define FPS_TARGET          60
#define INST_PER_FRAME      (FREQUENCY_MHZ / INSTRUCTION_COST / INST_PER_FRAME)

// --- MEMORY MAPPING OFFSETS ---
#define VRAM_OFST           0x04000000
#define FB_OFST             0x04000000
#define CART_RAM_OFST       0x03FE0000
#define RAM_OFST            0x0404B000
#define IO_OFST             0x0406B000
#define STACK_OFST          0x0FFFFFFF // To be defined


// --- JOYPAD ---
// Joypad address
#define JOYPAD_0            0x0406B000
// Define joypad masks
#define JOYPAD_UP           0x1 
#define JOYPAD_LEFT         0x2
#define JOYPAD_DOWN         0x4 
#define JOYPAD_RIGHT        0x8

extern const int COLORSPAL[32];


/**
 * @brief Loads the cartridge into rom pointer
 *  
 * WARNING: rom is allocated inside, do not forget to free
 * 
 * @param path 
 * @param rom 
 * @return int 
 */
int get_ptr_to_romdata(char *path, uint8_t **rom);




#endif