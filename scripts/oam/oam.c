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
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15
};


static int RED_TILE_INDEX = 2;
static int RED_TILE[64] = {
    0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC,
    0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC,
    0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC,
    0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC,
    0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC,
    0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC,
    0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC,
    0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC
};


static int PINK_TILE_INDEX = 3;
static int PINK_TILE[64] = {
    0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB,
    0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB,
    0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB,
    0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB,
    0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB,
    0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB,
    0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB,
    0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB
};

static int BLACK_TILE_INDEX = 4;
static int BLACK_TILE[64] = {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};


static int BLUE_TILE_INDEX = 5;
static int BLUE_TILE[64] = {
    0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,
    0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,
    0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,
    0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,
    0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,
    0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,
    0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,
    0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4
};

static int GREEN_TILE_INDEX = 6;
static int GREEN_TILE[64] = {
    0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16,
    0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16,
    0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16,
    0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16,
    0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16,
    0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16,
    0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16,
    0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16
};
static int JOYPAD_0 = 0x0406B000;


void init_tileset()
{
    int offset = 0;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = FUNKY_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = BLANK_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = RED_TILE[i];
    }
    offset+=64;



    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = PINK_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = BLACK_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = BLUE_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = GREEN_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = PINK_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = BLACK_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = BLUE_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = GREEN_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = PINK_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = BLACK_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = BLUE_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = GREEN_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = PINK_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = BLACK_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = BLUE_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = GREEN_TILE[i];
    }
}



void init_maps()
{
    for(int i = 0; i < 64*64*2; i+=2)
    {
        if(!(i % 7))
        {
            *(volatile unsigned char*)(MAP_0_ADDR + i) =     RED_TILE_INDEX & 0xFF;
            *(volatile unsigned char*)(MAP_0_ADDR + i + 1) = (RED_TILE_INDEX >> 8) & 0xFF;
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

    // 8x8 char
    new_addr = store_object(36, 33, FUNKY_TILE_INDEX, 0, 0x200, base_addr);
    base_addr = new_addr;

    // 8x16 char
    new_addr = store_object(128, 33, BLACK_TILE_INDEX, 0, 0x4, base_addr);
    base_addr = new_addr;

    // 8x32 char
    new_addr = store_object(200, 33, BLACK_TILE_INDEX, 0, 0x8, base_addr);
    base_addr = new_addr;



    // 16x8 char
    new_addr = store_object(36, 75, BLACK_TILE_INDEX, 0, 0xC, base_addr);
    base_addr = new_addr;

    // 16x16 char
    new_addr = store_object(128, 75, BLACK_TILE_INDEX, 0, 0x10, base_addr);
    base_addr = new_addr;

    // 16x32 char
    new_addr = store_object(200, 75, BLACK_TILE_INDEX, 0, 0x14, base_addr);
    base_addr = new_addr;



    // 32x8 char
    new_addr = store_object(36, 130, BLACK_TILE_INDEX, 0, 0x18, base_addr);
    base_addr = new_addr;

    // 32x16 char
    new_addr = store_object(128, 130, BLACK_TILE_INDEX, 0, 0x1C, base_addr);
    base_addr = new_addr;

    // 32x32 char
    new_addr = store_object(200, 130, BLACK_TILE_INDEX, 0, 0x20, base_addr);
    base_addr = new_addr;

}

int main() {

    init_tileset();
    init_maps();
    init_objects();
    int joypad;
    int current_sx;
    int current_sy;
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
                // Move BG
                // current_sx = *(volatile unsigned char*)(SCROLL_X);
                // *(volatile unsigned char*)(SCROLL_X) = current_sx + 1;
                
                // Move object 0
                current_sx = *(volatile unsigned char*)(OAM_ADDR);
                *(volatile unsigned char*)(OAM_ADDR) = current_sx + 1;
            }
            else if(joypad == 2)
            {
                // Move BG
                // current_sx = *(volatile unsigned char*)(SCROLL_X);
                // *(volatile unsigned char*)(SCROLL_X) = current_sx - 1;
                
                // Move object 0
                current_sx = *(volatile unsigned char*)(OAM_ADDR);
                *(volatile unsigned char*)(OAM_ADDR) = current_sx - 1;
            }
            else if(joypad == 4)
            {
                // Move BG
                // current_sy = *(volatile unsigned char*)(SCROLL_Y);
                // *(volatile unsigned char*)(SCROLL_Y) = current_sy + 1;
                
                // Move object 0
                current_sy = *(volatile unsigned char*)(OAM_ADDR + 2);
                *(volatile unsigned char*)(OAM_ADDR + 2) = current_sy + 1;
            }
            else if(joypad == 1)
            {
                // Move BG
                // current_sy = *(volatile unsigned char*)(SCROLL_Y);
                // *(volatile unsigned char*)(SCROLL_Y) = current_sy - 1;
                
                // Move object 0
                current_sy = *(volatile unsigned char*)(OAM_ADDR + 2);
                *(volatile unsigned char*)(OAM_ADDR + 2) = current_sy - 1;
            }
            // *(volatile unsigned char*)(MAP_INDEX) = *(volatile unsigned char*)(JOYPAD_0) % 2; 
        }
    }
    return 0;
}
