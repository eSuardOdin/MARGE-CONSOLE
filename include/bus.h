#ifndef BUS_H
#define BUS_H

#include "common.h"
#include "cartridge.h"
#include "object.h"
#include <stdint.h>

/**
 * @file bus.h
 * @brief Bus abstraction layer routing CPU read/write operations to memory and I/O devices.
 */


/**
 * @brief This struct is only the to abstract the communication between CPU (or other chips)
 * and Memory (or I/O devices)
 *
 * This struct is mainly used to route READ/WRITE operations to the good chip/memory location in regard to the Marge IO map.
 */
typedef struct
{

    /// Pointer to the cartridge plugged in console
    cartridge_t*    cartridge;
    /// The color indexes to display on screen
    uint8_t         framebuffer[0x9600];
    /// 128Kb of RAM mapped at **`0x0404B000 - 0x0406B000`**
    uint8_t         ram[0x20000];
    /// 32 Kb of stack mapped at **`0x05FF8000 - 0x06000000`**
    uint8_t         stack[0x8000];
    // --- IO memory region ---
    /// *-first?-* console's controller status register mapped at **`0x0406B000`**
    uint8_t         controller;
    // --- System registers ---
    /// Index register of the map (background) to display, mapped at **`0x0406B002`**
    uint8_t         map_index;
    /// X offset of the top left 240x160 displayed rectangle into the 512x512 map, mapped at **`0x0406B004`**
    uint8_t         scroll_x;
    /// X offset of the top left 240x160 displayed rectangle into the 512x512 map, mapped at **`0x0406B006`**
    uint8_t         scroll_y;
    /// Register incremented by console after a frame had been displayed **needs to be replaced with interrupts** mapped at `0x0406B0F0`
    uint8_t         frame_counter;
    /// 8x8 tiles, each point is a color index. 1024 possible tiles, mapped at **`0x0406C000 - 0x0407C000`** **2 bits wasted as we have a 32 colors palette** 
    uint8_t         tileset[0x10000];
    /// Collection of 64 screen of 64 * 64 tiles indexes, mapped at **`0x0406C000`**
    uint8_t         maps[MAP_BYTES];
    /// Object attribute memory, stores objects and there metadata. *(check `object_t` doc if needed)*
    uint8_t         oam[OBJECT_NUMBER * sizeof(object_t)];

} bus_t;

/**
 * @brief Initializes the bus linked memory areas.
 * 
 * @param bus Pointer to the bus to initialize - not allocated inside this function.
 * @param cart Pointer to the cartridge struct to link - not allocated inside this function.
 * @return int Error code
 */
int init_bus(bus_t* bus, cartridge_t* cart);

/**
 * @brief Switches the address in order to read into the console memory space or device registers mapped to the requested address.
 *
 * This checks if the address requested is corresponding to mapped device and returns 8bit of data associated
 * with the address. If the address is not mapped, this function just returns 0xFF and does not crashes the program.
 * It is the reponsability of the ROM/cartridge programmer (or of our *yet to come* user lib) to ensure it is requesting
 * a mapped address.
 * 
 * @param bus Pointer to the bus read from
 * @param addr 32 bit memory address to read from 
 * @return uint8_t Data read
 */
uint8_t read_memory(bus_t* bus, int32_t addr);


/**
 * @brief Switches the address in order to write the requested data into memory/device register mapped to the requested address
 * 
 * This checks if the requested address correspond to a writable space, if not, the program crashes **we may change it?**.
 * If the area is writable, data is put into register at requested address.
 * @param bus Pointer to the bus doing the write operation
 * @param data The 8 bit data we want to write 
 * @param addr The address to write data to
 */
void write_memory(bus_t* bus, uint8_t data, int32_t addr);


#endif