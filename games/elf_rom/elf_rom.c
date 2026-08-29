/* Goes in .data */
static int FB_ADDR = 0x04000000;
static int FB_SIZE = 0x4B000;
static int TIME = 0;
static int color = 13;
/* Goes in .rodata */
static const int static_constant_a = 42;
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
    // Everything enclosed in a function goes in stack
    for(int i = 0; i < FB_SIZE; i++)
    {
        *(volatile unsigned char*)(FB_ADDR + i) = (unsigned char)color;
    }
    int test;
    int ok = 3;
    for(int i = 0; i < 0x400; i++)
    {
        test += (i % 23) * 2;
        ok = test - i;
    }
    return 0;
}