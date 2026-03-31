
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


#define VRAM_OFST   0x04000000
#define FB_OFST     0x04000000
#define RAM_OFST    0x03FE0000
#define IO_OFST     0x0404B000


extern const int COLORSPAL[32];

#endif