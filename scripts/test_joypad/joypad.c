static int FB_ADDR = 0x04000000;
static int JOYPAD_0 = 0x0406B000;
static int FB_SIZE = 0x4B000;
static int TIME = 0;
static int color = 12;

void set_color(int color)
{
  for(int i = 0; i < FB_SIZE; i++)
      {
        *(volatile unsigned char*)(FB_ADDR + i) = (unsigned char)color;
      }
}


int main() {
  // Get joypad status
  int button = *(volatile unsigned char*)(JOYPAD_0);
  while(1)
  {
      set_color(button);
      button = *(volatile unsigned char*)(JOYPAD_0);
  }
  return 0;
}

