.globl _boot
_boot:
    li x2, 0x06000000
    call main
    ebreak
    j .

