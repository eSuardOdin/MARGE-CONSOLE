#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "common.h"
#include <stdint.h>

/**
 * @file cartridge.h
 * @brief Represents a Marge cartridge with it's ROM and RAM.
 */


/**
 * @brief Represents a Marge game cartridge
 * 
 */
typedef struct
{
    /// 128Kb of cartram. This space can be used to save data. **Warning, may be useless to do so as we are software, but want to represent some hardware to some extent**
    uint8_t     ram[0x20000];
    /// The instruction loaded from the ELF file, need to use a marge_header
    uint8_t*    rom;
    /// Size of the used ROM
    int         rom_size;
} cartridge_t;

/**
 * @brief Inits the cartridge and populates it's memory from the pointed to data.
 * 
 * @param cart Pointer to the cartridge to populate
 * @param rom Pointer to the read only memory (*got by loader.h from ELF executable*)
 * @param rom_size Size of the ROM
 * @return int Error code
 */
int init_cartridge(cartridge_t* cart, uint8_t* rom, size_t rom_size);

#endif