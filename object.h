#ifndef OBJECT_H
#define OBJECT_H

#include "common.h"

#define IS_X_INVERTED   0x1
#define IS_Y_INVERTED   0x2

// Bits[2,5] to mask against flag register
// in order to get object resolution
#define R_8X8             0x0
#define R_8X16            0x1
#define R_8X32            0x2
#define R_16X8            0x3
#define R_16X16           0x4
#define R_16X32           0x5
#define R_32X8            0x6
#define R_32X16           0x7
#define R_32X32           0x8

// X and Y offset, if object is (12,38), he is displayed as (-20, 6)
#define OFFSET            0x20
#define OBJECT_NUMBER     0x40

typedef struct object_t
{
    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t tile_index;
    uint16_t animation_sprites;
    uint16_t flags; // TBD
} object_t;


// Flag register definition
/**
 * 00:      is X axis flipped
 * 01:      is Y axis flipped
 * 02-05:   object resolution
 * 06-08:   object animation offset
 * // Add if any need
 *
 */


#endif