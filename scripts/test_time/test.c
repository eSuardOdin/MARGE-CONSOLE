static int FB_ADDR = 0x04000000;
static int FB_SIZE = 0x4B000;
static int TIME = 0;
static int color = 10;
int main() {
  int time = 0;
  for(int i = 0; i < FB_SIZE; i++)
  {
    *(volatile unsigned char*)(FB_ADDR + i) = (unsigned char)color;
  }
  while(1)
  {
    
   
    while(time != 0x20)
    {
      time = time + 1;
    }
    color = color == 31 ? 0 : color + 1;
    time = 0;
    for(int i = 0; i < FB_SIZE; i++)
    {
      *(volatile unsigned char*)(FB_ADDR + i) = (unsigned char)color;
    }
  }
  return 0;
}