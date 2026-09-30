.extern reset_handler
.extern frame_handler

.section ".text.boot"
.globl _boot
_boot:
    li x2, 0x06000000
    call main
    ebreak
    j .


.section ".vector_reset"
.globl vector_reset
vector_reset:
    j reset_handler

.section ".vector_frame"
.globl vector_frame
vector_frame:
    j frame_handler