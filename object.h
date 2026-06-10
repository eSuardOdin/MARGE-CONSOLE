#ifndef OBJECT_H
#define OBJECT_H

#include "common.h"

#define IS_X_INVERTED   0x1
#define IS_Y_INVERTED   0x2

// Bits[2,5] to mask against flag register
// in order to get object resolution
typedef enum e_object_resolution
{
    R_8X8,
    R_8X16,
    R_8X32, 
    R_16X8, 
    R_16X16,
    R_16X32,
    R_32X8,
    R_32X16,
    R_32X32
} e_object_resolution;
// #define R_8X8             0x0
// #define R_8X16            0x1
// #define R_8X32            0x2
// #define R_16X8            0x3
// #define R_16X16           0x4
// #define R_16X32           0x5
// #define R_32X8            0x6
// #define R_32X16           0x7
// #define R_32X32           0x8

// X and Y offset, if object is (12,38), he is displayed as (-20, 6)
#define OBJ_SCREEN_OFFSET 0x20
#define OBJECT_NUMBER     0x40

/**
 * @brief Defines an object as it is stored in OAM memory
 * 
 */
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
 * 06-08:   object animation offset (current tile to display)
 * 09:      if bit set, transparency is enabled (black color otherwise)
 * // Add if any need
 *
 */

 
/**
 * @brief Get the object resolution by using the object's flag
 * 
 * @param flag 
 * @param width 
 * @param height 
 * @return e_object_resolution 
 */
e_object_resolution get_object_resolution(int flag, int* width, int* height);

/**
 * @brief Get the object axis flip status by using object's flag
 * 
 * @param flag 
 * @param is_x_axis if true, checks X axis flip status, Y axis otherwise.
 * @return char 0 if not flipped 
 */
char get_object_flip(int flag, char is_x_axis);

/**
 * @brief Get the object current frame to display (an offset from the tile index of the object)
 * 
 * @param flag 
 * @return int 
 */
char get_object_frame(int flag);

/**
 * @brief Get the current object transparency enabling value
 * 
 * @param flag 
 * @return char 
 */
char is_transparency_enabled(int flag);


#endif