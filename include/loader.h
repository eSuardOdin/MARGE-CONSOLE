#ifndef LOADER_H
#define LOADER_H

#include "common.h"
#include "cartridge.h"
#include "bus.h"

/**
 * @brief Checks if the file is a valid ELF (and an executable elf type), returns a pointer to
 * the elf header 
 * 
 * @param fp - a pointer to the file provided as program argument.
 * @return Elf64_Ehdr* - a pointer to the ELF header, NULL if invalid.
 */
Elf64_Ehdr* get_elf_from_file(FILE* fp);

/**
 * @brief Loads the defined sections from the ELF file to the console and cartridge.
 * We copy the .data, .rodata in console's RAM but a section is defined to go into
 * cartridge's RAM (which needs to be defined as writable obviously)
 * 
 * @param p_elf_header The header we get sections/segments info
 * @param fp The pointer to the actual ELF file
 * @param p_bus The console's BUS - it IS holding the console's whole IO map
 * @param p_cart Cart we load ROM and RAM to.
 * @return int 0 if no errors - TODO define error codes
 */
int load_elf(Elf64_Ehdr* p_elf_header, FILE* fp, bus *p_bus, cartridge_t *p_cart);


#endif