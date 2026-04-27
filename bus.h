#ifndef BUS_H
#define BUS_H

#include "common.h"
#include "cartridge.h"

/**
 * @brief This struct is only the to abstract the communication between CPU (or other chips)
 * and Memory (or I/O devices)
 * 
 */
typedef struct bus_t
{
    cartridge_t*    cartridge;
    uint8_t*        vram;
    uint8_t         framebuffer[0x4B000];
    uint8_t         ram[0x2000];
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
int dump_memory(bus_t* bus, int start, int size); 


#endif