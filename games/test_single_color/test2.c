static int FB_ADDR = 0x04000000;
static int FB_SIZE = 0x4B000;
static int TIME = 0;
static int color = 13;
static const int static_constant_in_rodata = 42;
__attribute__((section(".cartram"))) char save_date[128*1024];
int main() {
  for(int i = 0; i < FB_SIZE; i++)
  {
    *(volatile unsigned char*)(FB_ADDR + i) = (unsigned char)color;
  }
  return 0;
}