#ifndef LOADER_H
#define LOADER_H

#include "common.h"
#include "cartridge.h"
#include "bus.h"
#include "cpu.h"

#include <elf.h>

/**
 * @brief Checks if the file is a valid ELF (and an executable elf type), returns a pointer to
 * the elf header 
 * 
 * @param filepath - a path to the file provided as program argument.
 * @return FILE* - a pointer to the FILE pointer when identified as a valid ELF, NULL if invalid.
 */
FILE* get_elf_file(char* filepath);


int init_console(FILE* executable, cartridge_t* cart, cpu_t* cpu, bus* bus);


/**
 * @brief Get the section by a string, returns NULL if not found
 * 
 * @param executable The file pointer to ELF
 * @param section_name The name of the section to find
 * @return Elf32_Shdr* A pointer to the section header requested
 */
Elf32_Shdr* get_section_header_by_name(FILE* executable, Elf32_Ehdr* elf_header, char* section_name);


/**
 * @brief Just an fseek()/fread() wrapper, the pointer needs to be casted to the wanted type
 * 
 * @param executable The ELF file to read from
 * @param offset The offset to fseek() to
 * @param size The size of the struct we read to
 * @param n_size The number of contiguous structs we read to
 * @return void* A pointer to cast as the intended return type (eg. marge_header struct pointer) - WARNING: The struct pointer must be freed when not used anymore.
 */
void* extract_from_elf(FILE* executable, long offset, size_t size, int n_size);

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