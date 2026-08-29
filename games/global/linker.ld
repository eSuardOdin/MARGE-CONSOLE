ENTRY(_boot)
MEMORY
{
    CARTROM (rx)        : ORIGIN = 0x0, LENGTH = 65408K
    CARTRAM (xrw)       : ORIGIN = 0x03FE0000, LENGTH = 128K
    RAM (xrw)           : ORIGIN = 0x0404B000, LENGTH = 128K
    STACK (xrw)         : ORIGIN = 0x05FF8000, LENGTH = 32K
}
SECTIONS {
    /* Start address of the program */
    . = 0x0000;

    /* Custom header to check if valid cartridge */
    .marge_header : {
        *(.marge_header)
    } > CARTROM

    /* Program instructions in cartridge */
    .text : {
        *(.text)
    } > CARTROM

    /* Readonly constants defined in code, makes sens to put it in ROM */
    .rodata : {
        *(.rodata)
    } > CARTROM

    /* Cartridge saving "chip" */
    . = 0x3FE0000;
    .cartram : {
        *(.cartram)
    } > CARTRAM

    . = 0x404B000; /* Jumping to beginning of console's RAM */

    /* Initialized R/W data */
    .data : {
        *(.data)
    } > RAM
    
    /* Uninitialized R/W data */
    .bss : {
        *(.bss)
    } > RAM

    /* Discarding "useless" sections
    .comment            : Just some info data about the compiler
    .eh_frame           : Used to unwind exceptions and go up the callstack, will not use it.
    .note.GNU-stack     : Tells if the stack is executable 
                          (used by the OS to decide the RWX permissions, but WE are the "OS" and do the mapping) 
    .riscv.attributes   : Informations about the instruction set etc.. We know all we need as the architecture we
                          target is ours.
    */
    /DISCARD/ : { 
        *(.note.GNU-stack) 
        *(.comment)
        *(.eh_frame)
        *(.riscv.attributes)
        }
}
