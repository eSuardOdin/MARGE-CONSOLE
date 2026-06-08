static const int  FRAME_COUNTER = 0x0406B0F0;
static const int OAM_ADDR = 0x040FC000;
static const int OBJ_SIZE = 0xA;
static const int OBJ_NUMBER = 0x40;

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
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x14, 0x14, 0x14, 0x14, 0x15, 0x15,
    0x15, 0x14, 0x07, 0x15, 0x15, 0x07, 0x15, 0x14,
    0x14, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x14,
    0x14, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x14,
    0x14, 0x15, 0x0D, 0x0D, 0x0D, 0x0D, 0x14, 0x15,
    0x15, 0x14, 0x15, 0x15, 0x15, 0x14, 0x15, 0x15,
    0x15, 0x15, 0x14, 0x14, 0x14, 0x15, 0x15, 0x15
};
// Tile at index 1
static int BLANK_TILE_INDEX = 1;
static int BLANK_TILE[64] = {
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15
};


static int JOYPAD_0 = 0x0406B000;


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
        // if(!(i % 8))
        // {
        //     *(volatile unsigned char*)(MAP_0_ADDR + i) =     FUNKY_TILE_INDEX & 0xFF;
        //     *(volatile unsigned char*)(MAP_0_ADDR + i + 1) = (FUNKY_TILE_INDEX >> 8) & 0xFF;
        // }
        // else
        // {
            *(volatile unsigned char*)(MAP_0_ADDR + i) = BLANK_TILE_INDEX & 0xFF;
            *(volatile unsigned char*)(MAP_0_ADDR + i + 1) = (BLANK_TILE_INDEX >> 8) & 0xFF;
        // }
    }

    for(int i = 0; i < 64*64*2; i+=2)
    {
       *(volatile unsigned char*)(MAP_1_ADDR + i) = BLANK_TILE_INDEX & 0xFF;
       *(volatile unsigned char*)(MAP_1_ADDR + i + 1) = (BLANK_TILE_INDEX >> 8) & 0xFF;
    }
}



void store_sixteenth(int data, int addr)
{
    *(volatile unsigned char*)(addr)   = data & 0xFF;
    *(volatile unsigned char*)(addr+1) = (data & 0xFF00) >> 8;
}


int store_object(int x, 
    int y, 
    int tile_index, 
    int animation_sprites, 
    int flags,
    int base_addr)
{
    store_sixteenth(x, base_addr);
    store_sixteenth(y, base_addr+2);
    store_sixteenth(tile_index, base_addr+4);
    store_sixteenth(animation_sprites, base_addr+6);
    store_sixteenth(flags, base_addr+8);

    return base_addr + OBJ_SIZE;
}



void init_objects()
{
    int base_addr = OAM_ADDR;
    int new_addr;
    // for(int i = 0; i < OBJ_NUMBER; i++)
    // {
    //     new_addr = store_object(i+3, i*2, i+24, i%5, i, base_addr);
    //     base_addr = new_addr;
    // }

    new_addr = store_object(36, 36, 0, 0, 0, base_addr);
    base_addr = new_addr;
    new_addr = store_object(128, 74, 1, 2, 0x4, base_addr);
    base_addr = new_addr;
    new_addr = store_object(128, 74, 1, 2, 0x8, base_addr);
    base_addr = new_addr;

    new_addr = store_object(128, 74, 1, 2, 0xC, base_addr);
    base_addr = new_addr;
    new_addr = store_object(128, 74, 1, 2, 0x10, base_addr);
    base_addr = new_addr;
    new_addr = store_object(128, 74, 1, 2, 0x14, base_addr);
    base_addr = new_addr;

    new_addr = store_object(128, 74, 1, 2, 0x18, base_addr);
    base_addr = new_addr;
    new_addr = store_object(128, 74, 1, 2, 0x1C, base_addr);
    base_addr = new_addr;
    new_addr = store_object(128, 74, 1, 2, 0x20, base_addr);
    base_addr = new_addr;

}

int main() {

    init_tileset();
    init_maps();
    init_objects();
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
