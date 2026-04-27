
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


#define VRAM_OFST           0x04000000
#define FB_OFST             0x04000000
#define CART_RAM_OFST       0x03FE0000
#define RAM_OFST            0x0404B000
#define IO_OFST             0x0406B000


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