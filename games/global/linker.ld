ENTRY(_boot)
SECTIONS {
    /* Start address of the program */
    . = 0x0000;
    /* Program code starts here */
    .text : {
        *(.text)
    }
    . = 0x404B000;
    /* Initialized data starts here */
    .data : {
        *(.data)
    }
    . = 0x404C000;
    /* Uninitialized data starts here */
    .bss : {
        *(.bss)
    }
    /* Discard stack section */
    /DISCARD/ : { *(.note.GNU-stack) }
}
