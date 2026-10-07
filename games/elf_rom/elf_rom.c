#include "../../include/common.h"

/* Goes in .data */
static const int frame_counter_address = 0x0406B0F0;
static int num = 1;
static int saved_frame = 0;
static int current_frame = 0;
static int a = 78235;
static int b = 0;
static int c = 0;
static int d = 0;
static int e = 0;
int joypad = 0;
int debug_reg = 0;
/* Goes in .rodata */
static const int static_constant_a = 0x1;
static const char static_constant_b = 'c';
/* Goes in .cartram */
__attribute__((section(".cartram"))) char save_date[128*1024];

/* Goes in .marge_header */
__attribute__((section(".marge_header"))) const struct marge_header {
    char magic_number[10];
    char title[32];
    char author[32];
    char maj_version;
    char min_version;
    char rev_version;
} header = {"Marge_Sys\0", "ELF TESTING ROM", "Marge Corp", 0, 2, 55};

int main() {
    *(volatile unsigned char*)(INTERRUPT_REGISTER) = 1;
    while(1) 
    {
        current_frame = *(volatile unsigned char*)(frame_counter_address);
        if(current_frame != saved_frame)
        {
            //num += static_constant_a;
            *(volatile unsigned char*)(DEBUG_REG) = current_frame;
        }
    }
    return 0;
}

void update_object() {
    joypad = *(volatile unsigned char*)(JOYPAD_0);
    if (joypad & JOYPAD_RIGHT) {
        char* error = "GROS CACA QUI PUE\0";
        int i = 0;
        do {
            *(volatile unsigned char*)(DEBUG_REG + i) = error[i];
        } while (error[i++] != '\0');
    }
}

__attribute__((interrupt))
void reset_handler()
{
    main();
}

__attribute__((interrupt))
void frame_handler()
{
    update_object();
    *(volatile unsigned char*)(INTERRUPT_REGISTER) = 1;
}


void update_timer()
{
    
}

__attribute__((interrupt))
void timer_handler()
{
    update_timer();
}
