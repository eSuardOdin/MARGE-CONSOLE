#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <errno.h>
#include <string.h>

#include "loader.h"


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
    Elf32_Shdr string_table_section_header;
    long offset = elf_header.e_shoff + elf_header.e_shstrndx * elf_header.e_shentsize;
    if((err = fseek(fp, offset, SEEK_SET)) < 0)
    {
        fprintf(stderr, "get_elf_file() - Unable to fseek to string table section header : [Code:%d] - %s\n", errno, strerror(errno));
        exit(errno);
    }
    errno = 0;
    bytes_read = fread(&string_table_section_header, sizeof(string_table_section_header), 1, fp);
    if(bytes_read != 1)
    {
        err = ferror(fp);
        fprintf(stderr, "get_elf_file() - Error when reading the file for string table section header : [Code:%d]\n", err);
        exit(err);
    }
    if(string_table_section_header.sh_type != SHT_STRTAB)
    {
        fprintf(stderr, "get_elf_file() - Error when reading the file for string table section header : Table type error\n\tWaiting [%d] Got [%d]\n", err, SHT_STRTAB, string_table_section_header.sh_type );
        exit(err);
    }
    char* str_tab = malloc(string_table_section_header.sh_size);
    if((err = fseek(fp, string_table_section_header.sh_offset, SEEK_SET)) < 0)
    {
        fprintf(stderr, "get_elf_file() - Unable to fseek to string table raw string : [Code:%d] - %s\n", errno, strerror(errno));
        exit(errno);
    }
    bytes_read = fread(str_tab, sizeof(char), string_table_section_header.sh_size, fp);
    if(bytes_read == 0)
    {
        err = ferror(fp);
        fprintf(stderr, "get_elf_file() - Error when reading the file for string table section raw string : [Code:%d]\n", err);
        exit(err);
    }
    // str_tab is now populated of string table

    // Get the .marge_header section
    Elf32_Shdr section_header;
    errno = 0;
    for(int i = 1; i < elf_header.e_shnum; i++)
    {
        // Get to section header file offset
        offset = elf_header.e_shoff + i * elf_header.e_shentsize;
        if((err = fseek(fp, offset, SEEK_SET)) < 0)
        {
            fprintf(stderr, "get_elf_file() - Unable to fseek to section header n°%d : [Code:%d] - %s\n", i, errno, strerror(errno));
            exit(errno);
        }
        bytes_read = fread(&section_header, sizeof(section_header), 1, fp);
        if(bytes_read != 1)
        {
            int err = ferror(fp);
            fprintf(stderr, "get_elf_file() - Error when reading the file for section headers : [Code:%d]\n", err);
            exit(err);
        }
        if(section_header.sh_type && string_table_section_header.sh_name)
        {
            fprintf(stdout, "Section n°%d: %s\n", i , str_tab + section_header.sh_name);
        }
    }

    fprintf(stdout, "File successfully loaded.\n");
    return fp;

}