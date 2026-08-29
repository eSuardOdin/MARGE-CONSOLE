#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <errno.h>
#include <string.h>

#include "loader.h"


FILE* get_elf_file(char* filepath)
{
    errno = 0;
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
        int err = ferror(fp);
        fprintf(stderr, "Error when reading the file : [Code:%d]\n", err);
        exit(err);
    }

    // Check if valid executable elf 
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

    fprintf(stdout, "File successfully loaded.\n");
    return fp;

}