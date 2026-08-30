#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <errno.h>
#include <string.h>

#include "loader.h"
#include "common.h"


FILE* get_elf_file(char* filepath)
{
    errno = 0;
    int err;
    FILE* fp = fopen(filepath, "rb");
    if(fp == NULL)
    {
        fprintf(stderr, "Unable to open '%s' : [Code:%d] - %s\n", filepath, errno, strerror(errno));
        exit(errno);
    }

    Elf32_Ehdr elf_header;
    size_t bytes_read = fread(&elf_header, sizeof(Elf32_Ehdr), 1, fp);
    if(bytes_read != 1)
    {
        err = ferror(fp);
        fprintf(stderr, "Error when reading the file : [Code:%d]\n", err);
        exit(err);
    }

    /* Check if valid executable elf */
    if( elf_header.e_ident[EI_MAG0] != ELFMAG0 ||
        elf_header.e_ident[EI_MAG1] != ELFMAG1 ||
        elf_header.e_ident[EI_MAG2] != ELFMAG2 ||
        elf_header.e_ident[EI_MAG3] != ELFMAG3)
    {
        fprintf(stderr, "File %s is not an ELF file.\n", filepath);
        exit(EXIT_FAILURE);
    }

    if(elf_header.e_type != ET_EXEC)
    {
        fprintf(stderr, "File %s is not an executable ELF.\n", filepath);
        exit(EXIT_FAILURE);
    }

    /* Checking the custom console header */

    // Get the string table header
    errno = 0;
    Elf32_Shdr* string_table_section_header = malloc(sizeof(Elf32_Shdr));
    if(string_table_section_header == NULL)
    {
        fprintf(stderr, "get_elf_file() - Unable to allocate string_table_section_header : [Code:%d] - %s\n", errno, strerror(errno));
        exit(errno);
    }
    string_table_section_header = (Elf32_Shdr*)extract_from_elf(fp, elf_header.e_shoff + elf_header.e_shstrndx * elf_header.e_shentsize, sizeof(Elf32_Shdr), 1);
    if(string_table_section_header->sh_type != SHT_STRTAB)
    {
        free(string_table_section_header);
        fprintf(stderr, "get_elf_file() - Error when reading the file for string table section header : Table type error\n\tWaiting [%d] Got [%d]\n", err, SHT_STRTAB, string_table_section_header->sh_type );
        exit(err);
    }

    // Get the raw string table
    errno = 0;
    char* str_tab = (char*)extract_from_elf(fp, string_table_section_header->sh_offset, sizeof(char), string_table_section_header->sh_size);
    if(str_tab == NULL)
    {
        fprintf(stderr, "get_elf_file() - Unable to allocate str_tab : [Code:%d] - %s\n", errno, strerror(errno));
        exit(errno);
    }

    // Get the .marge_header section header
    Elf32_Shdr* section_header = get_section_header_by_name(fp, &elf_header, ".marge_header");
    if(section_header == NULL)
    {
        fprintf(stderr, "get_elf_file() - Error when get marge_header section header.\n");
        exit(err);
    }

    errno = 0;
    if((err = fseek(fp, section_header->sh_offset, SEEK_SET)) < 0)
    {
        fprintf(stderr, "get_elf_file() - Unable to get marge header section : [Code:%d] - %s\n", errno, strerror(errno));
        exit(errno);
    }
    // Read the full marge_header struct
    errno = 0;
    marge_header* m_header = (marge_header*)extract_from_elf(fp, section_header->sh_offset, sizeof(marge_header), 1);
    printf("Magic number: %s\nROM Title: %s\nAuthor: %s\nVersion: %d.%d.%d\n", 
        m_header->magic_number,
        m_header->title,
        m_header->author,
        m_header->maj_version,
        m_header->min_version,
        m_header->rev_version
    );
    if((err = fseek(fp, 0L, SEEK_SET)) < 0)
    {
        fprintf(stderr, "get_elf_file() - Unable to rewind ELF file\n", errno, strerror(errno));
        exit(errno);
    }
    free(section_header);
    free(m_header);
    free(str_tab);
    return fp;
}




Elf32_Shdr* get_section_header_by_name(FILE* executable, Elf32_Ehdr* elf_header, char* section_name)
{
    // Get the string table header
    long offset = elf_header->e_shoff + elf_header->e_shstrndx * elf_header->e_shentsize;
    Elf32_Shdr* string_table_section_header = (Elf32_Shdr*)extract_from_elf(executable, offset, sizeof(Elf32_Shdr), 1);
    if(string_table_section_header->sh_type != SHT_STRTAB)
    {
        fprintf(stderr, "get_section_header_by_name() - Error when reading the file for string table section header : Table type error\n\tWaiting [%d] Got [%d]\n", SHT_STRTAB, string_table_section_header->sh_type );
        exit(EXIT_FAILURE);
    }
    errno = 0;
    char* str_tab = (char*)extract_from_elf(executable, string_table_section_header->sh_offset, sizeof(char), string_table_section_header->sh_size);

    // Get the section
    errno = 0;
    Elf32_Shdr* section_header = NULL;
    errno = 0;
    for(int i = 1; i < elf_header->e_shnum; i++)
    {
        // Get to section header file offset
        offset = elf_header->e_shoff + i * elf_header->e_shentsize;
        section_header = (Elf32_Shdr*)extract_from_elf(executable, offset, sizeof(Elf32_Shdr), 1);
        if(string_table_section_header->sh_name && !strcmp(str_tab + section_header->sh_name, section_name))
        {
            free(string_table_section_header);
            return section_header;
        }
    }
    free(string_table_section_header);
    return NULL;
}




void* extract_from_elf(FILE* executable, long offset, size_t size, int n_size)
{
    int err;
    size_t bytes_read;
    errno = 0;
    void* return_pointer = malloc(size * n_size);
    errno = 0;
    if((err = fseek(executable, offset, SEEK_SET)) < 0)
    {
        fprintf(stderr, "extract_from_elf() - Unable to fseek() to the wanted structure : [Code:%d] - %s\n", errno, strerror(errno));
        free(return_pointer);
        exit(errno);
    }
    bytes_read = fread(return_pointer, size, n_size, executable);
    if(bytes_read == 0)
    {
        err = ferror(executable);
        fprintf(stderr, "extract_from_elf() - Error when reading the file : [Code:%d]\n", err);
        free(return_pointer);
        exit(err);
    }

    return return_pointer;
}




int load_cartridge(cartridge_t* cart, FILE* executable)
{
    Elf32_Ehdr elf_header;
    int err;
    size_t bytes_read = fread(&elf_header, sizeof(Elf32_Ehdr), 1, executable);
    if(bytes_read != 1)
    {
        err = ferror(executable);
        fprintf(stderr, "Error when reading the file : [Code:%d]\n", err);
        exit(err);
    }

    size_t rom_size = 0;
    // Get all headers and the full size of ROM sections (.marge_header, .text, .rodata)
    Elf32_Shdr* marge_header = get_section_header_by_name(executable, &elf_header, ".marge_header");
    Elf32_Shdr* text = get_section_header_by_name(executable, &elf_header, ".text");
    Elf32_Shdr* rodata = get_section_header_by_name(executable, &elf_header, ".rodata");

    rom_size += marge_header->sh_size + text->sh_size + rodata->sh_size;
    errno = 0;
    uint8_t* rom = malloc(rom_size);
    if(rom == NULL)
    {
        fprintf(stderr, "load_cartridge() - Unable to allocate rom : [Code:%d] - %s\n", errno, strerror(errno));
        exit(errno);
    }

    // Memcopy all sections
    uint8_t* src = extract_from_elf(executable, marge_header->sh_offset, marge_header->sh_size, 1);
    memcpy(rom, src, marge_header->sh_size);
    src = extract_from_elf(executable, text->sh_offset, text->sh_size, 1);
    memcpy(rom + marge_header->sh_size, src, text->sh_size);
    src = extract_from_elf(executable, rodata->sh_offset, rodata->sh_size, 1);
    memcpy(rom + marge_header->sh_size + text->sh_size, src, rodata->sh_size);

    init_cartridge(cart, rom, rom_size);
    return 0;
}