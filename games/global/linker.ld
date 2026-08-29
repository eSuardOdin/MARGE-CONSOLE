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
    /* Program instructions in cartridge */
    .text : {
        *(.text)
    } > CARTROM
    .cartram : {
        *(.cartram)
    } > CARTRAM
    . = 0x404B000; /* Beginning of console's RAM */
    /* Initialized data */
    .data : {
        *(.data)
    } > RAM
    . = 0x404C000;
    /* Uninitialized data starts here */
    .bss : {
        *(.bss)
    } > RAM
    /* Discard stack section */
    /DISCARD/ : { *(.note.GNU-stack) }
}
