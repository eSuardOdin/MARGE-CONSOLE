.globl _boot
_boot:
    li x2, 0x03FEFFFF
    call main
    ebreak
    j .

