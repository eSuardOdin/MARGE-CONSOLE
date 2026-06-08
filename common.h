
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

#define SCREEN_WIDTH        240
#define SCREEN_HEIGHT       160
#define SCALE               3


// --- CPU SPECS ---
#define FREQUENCY_MHZ       16800000
#define INSTRUCTION_COST    1
#define FPS_TARGET          60
#define INST_PER_FRAME      (FREQUENCY_MHZ / INSTRUCTION_COST / FPS_TARGET)

// --- MEMORY MAPPING OFFSETS AND STARTING POINT ---
#define CART_RAM_OFST       0x03FE0000
#define VRAM_OFST           0x04000000
#define FB_OFST             0x04000000
#define RAM_OFST            0x0404B000
#define IO_OFST             0x0406B000
#define TILESET_OFST        0x0406C000
#define MAPS_OFST           0x0407C000

#define OAM_OFST            0x040FC000
#define STACK_OFST          0x0FFFFFFF // To be defined


// --- JOYPAD ---
// Joypad address
#define JOYPAD_0            0x0406B000
// Define joypad masks
#define JOYPAD_UP           0x1 
#define JOYPAD_LEFT         0x2
#define JOYPAD_DOWN         0x4 
#define JOYPAD_RIGHT        0x8


// --- System Registers ---
// Defines wich map is to be printed
#define MAP_INDEX           0x0406B002
#define SCROLL_X            0x0406B004 // Scroll of BG (X OFFSET)
#define SCROLL_Y            0x0406B006 // Scroll of BG (Y OFFSET)


#define FRAME_COUNTER       0x0406B0F0


// --- Utility ---
#define MAP_BYTES           0x80000   
#define TILE_SIZE           0x40

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