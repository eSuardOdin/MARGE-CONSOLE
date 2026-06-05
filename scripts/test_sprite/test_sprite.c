static const int  FRAME_COUNTER = 0x0406B0F0;

static const int FB_ADDR = 0x04000000;
static const int SCROLL_X = 0x0406B004;
static const int SCROLL_Y = 0x0406B006;
static const int TILESET_ADDR = 0x0406C000;
static const int  MAP_INDEX = 0x0406B002;

static const int MAP_0_ADDR = 0x0407C000;
static const int MAP_0[64*64];


static const int MAP_1[64*64];
static const int MAP_1_ADDR = 0x0407E000;

// Tile at index 0
static int FUNKY_TILE_INDEX = 0;
static int FUNKY_TILE[64] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00,
    0x00, 0x14, 0x07, 0x00, 0x00, 0x07, 0x00, 0x14,
    0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14,
    0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14,
    0x14, 0x00, 0x0D, 0x0D, 0x0D, 0x0D, 0x14, 0x00,
    0x00, 0x14, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00,
    0x00, 0x00, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00
};
// Tile at index 1
static int BLANK_TILE_INDEX = 1;
static int BLANK_TILE[64] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


static int JOYPAD_0 = 0x0406B000;


// awfull but fuck it
void init_tileset()
{
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i) = FUNKY_TILE[i];
    }
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + 64) = BLANK_TILE[i];
    }
}



void init_maps()
{
    for(int i = 0; i < 64*64*2; i+=2)
    {
        if(!(i % 5))
        {
            *(volatile unsigned char*)(MAP_0_ADDR + i) =     FUNKY_TILE_INDEX & 0xFF;
            *(volatile unsigned char*)(MAP_0_ADDR + i + 1) = (FUNKY_TILE_INDEX >> 8) & 0xFF;
        }
        else
        {
            *(volatile unsigned char*)(MAP_0_ADDR + i) = BLANK_TILE_INDEX & 0xFF;
            *(volatile unsigned char*)(MAP_0_ADDR + i + 1) = (BLANK_TILE_INDEX >> 8) & 0xFF;
        }
    }

    for(int i = 0; i < 64*64*2; i+=2)
    {
       *(volatile unsigned char*)(MAP_1_ADDR + i) = BLANK_TILE_INDEX & 0xFF;
       *(volatile unsigned char*)(MAP_1_ADDR + i + 1) = (BLANK_TILE_INDEX >> 8) & 0xFF;
    }
}



int main() {

    init_tileset();
    init_maps();
    int joypad;
    int current_sx;
    int saved_frame = 0;
    int current_frame = 0;
    while(1)
    {

        // Get frame
        current_frame = *(volatile unsigned char*)(FRAME_COUNTER); 
        if(saved_frame != current_frame)
        {
            saved_frame = current_frame;
            joypad = *(volatile unsigned char*)(JOYPAD_0); 
            if(joypad == 8)
            {
                current_sx = *(volatile unsigned char*)(SCROLL_X);
                *(volatile unsigned char*)(SCROLL_X) = current_sx + 1;
            }
            else if(joypad == 2)
            {
                current_sx = *(volatile unsigned char*)(SCROLL_X);
                *(volatile unsigned char*)(SCROLL_X) = current_sx - 1;
            }
            *(volatile unsigned char*)(MAP_INDEX) = *(volatile unsigned char*)(JOYPAD_0) % 2; 
        }
    }
    return 0;
}
