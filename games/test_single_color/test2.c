static int FB_ADDR = 0x04000000;
static int FB_SIZE = 0x4B000;
static int TIME = 0;
static int color = 13;
static const int hello_ma_couille = 0xFFFF;
int main() {
  for(int i = 0; i < FB_SIZE; i++)
  {
    *(volatile unsigned char*)(FB_ADDR + i) = (unsigned char)color;
  }
  return 0;
}