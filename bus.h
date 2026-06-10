#ifndef BUS_H
#define BUS_H

#include "common.h"
#include "cartridge.h"
#include "object.h"

/**
 * @brief This struct is only the to abstract the communication between CPU (or other chips)
 * and Memory (or I/O devices)
 */
typedef struct bus_t
{
    // data struct for the cartridge loaded in console
    cartridge_t*    cartridge;
    uint8_t*        vram;   // May need to remove this one, duplicate with framebuffer
    // Pixels that will be printed on screen each frame
    uint8_t         framebuffer[0x9600];
    // Console ram
    uint8_t         ram[0x2000];
    // --- IO memory region ---
    // The console -first?- controller
    uint8_t         controller;
    // --- System registers ---
    uint8_t         map_index;
    uint8_t         scroll_x;
    uint8_t         scroll_y;
    uint8_t         frame_counter;

    // Tiles the console can display, stored as an 8x8 array of color indexes
    uint8_t         tileset[0x10000];
    // Collection of 64 screen of 64 * 64 tiles indexes (  )
    uint8_t         maps[MAP_BYTES];

    uint8_t         oam[OBJECT_NUMBER * sizeof(object_t)];

} bus_t;

int init_bus(bus_t* bus, cartridge_t* cart);

uint8_t read_memory(bus_t* bus, int32_t addr);
void write_memory(bus_t* bus, uint8_t data, int32_t addr);

/**
 * @brief Just dumps chunk of memory, debugging purpose.
 *
 * WARNING : No security whatsoever, may crash if bad address or size.
 * 
 * @param bus Pointer to the bus
 * @param start Start address to dump
 * @param size Number of adresses to dump
 * @return int status
 */
// int dump_memory(bus_t* bus, int start, int size); 


#endif