/* Goes in .data */
static const int  FRAME_COUNTER = 0x0406B0F0;
static int num = 1;
static int saved_frame = 0;
static int current_frame = 0;
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
    while(1) 
    {
        current_frame = *(volatile unsigned char*)(FRAME_COUNTER);
        if(current_frame != saved_frame)
        {
            //num += static_constant_a;
            saved_frame = current_frame;
        }
    }
    return 0;
}