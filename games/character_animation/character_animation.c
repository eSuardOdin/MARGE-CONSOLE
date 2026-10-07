static const int  FRAME_COUNTER = 0x0406B0F0;
static const int OAM_ADDR = 0x040FC000;
static const int OBJ_SIZE = 0xA;
static const int OBJ_NUMBER = 0x40;

static const int FB_ADDR = 0x04000000;
static const int SCROLL_X = 0x0406B004;
static const int SCROLL_Y = 0x0406B006;
static const int IME = 0x0406B100;
static const int TILESET_ADDR = 0x0406C000;
static const int  MAP_INDEX = 0x0406B002;

static const int MAP_0_ADDR = 0x0407C000;
static const int MAP_0[64*64];


static const int MAP_1[64*64];
static const int MAP_1_ADDR = 0x0407E000;

#define TEST 2

static int MAX_ANIMATION_OFFSET = 3;
static int CURRENT_ANIMATION_OFFSET = 0;
static int FRAME_PACE = 8;
static int CHANNEL_ADDR = 0x040FD404;
static short g_X = 50;
static short g_Y = 50;
static char is_moving = 0;
int joypad;
int current_sx;
int current_sy;
int remaining_frames;


// First 2 8x8 are mirrored idle
static int TOP_LEFT_0_INDEX = 1;
static char TOP_LEFT_0[64] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1,
    0x00, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1, 0x1,
    0x00, 0x00, 0x00, 0x1, 0x1, 0x1, 0x1, 0x1,
    0x00, 0x00, 0x00, 0x1, 0x1, 0x13, 0x13, 0x1,
    0x00, 0x00, 0x00, 0x1, 0x1, 0x05, 0x05, 0x13,
    0x00, 0x00, 0x00, 0x1, 0x13, 0x05, 0x08, 0x13,
};

static int BOT_LEFT_0_INDEX = 2;
static char BOT_LEFT_0[64] = {
    0x00, 0x00, 0x00, 0x1, 0x13, 0x13, 0x13, 0x13,
    0x00, 0x00, 0x00, 0x00, 0x1, 0x13, 0x13, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1,
    0x00, 0x00, 0x00, 0x00, 0x1, 0x05, 0x13, 0x13,
    0x00, 0x00, 0x00, 0x1, 0x13, 0x13, 0x05, 0x13,
    0x00, 0x00, 0x00, 0x1, 0x13, 0x1, 0x05, 0x08,
    0x00, 0x00, 0x00, 0x00, 0x1, 0x1, 0x05, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x05, 0x1,
};

// Left side first change from idle (top can be mirrored for second change from idle)
static int TOP_LEFT_1_INDEX = 3;
static char TOP_LEFT_1[64] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1,
    0x00, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1, 0x1,
    0x00, 0x00, 0x00, 0x1, 0x1, 0x1, 0x1, 0x1,
    0x00, 0x00, 0x00, 0x1, 0x1, 0x13, 0x13, 0x1,
    0x00, 0x00, 0x00, 0x1, 0x1, 0x05, 0x05, 0x13,
    0x00, 0x00, 0x00, 0x1, 0x13, 0x05, 0x08, 0x13,
    0x00, 0x00, 0x00, 0x1, 0x13, 0x13, 0x13, 0x13,
};

static int BOT_LEFT_1_INDEX = 4;
static char BOT_LEFT_1[64] = {
    0x00, 0x00, 0x00, 0x00, 0x1, 0x13, 0x13, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1,
    0x00, 0x00, 0x00, 0x00, 0x1, 0x05, 0x13, 0x13,
    0x00, 0x00, 0x00, 0x1, 0x13, 0x13, 0x05, 0x13,
    0x00, 0x00, 0x00, 0x1, 0x1, 0x1, 0x05, 0x08,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x05, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x05, 0x1,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x05, 0x1,
};

static int TOP_LEFT_2_INDEX = 5;
static int BOT_LEFT_2_INDEX = 6;
static char BOT_LEFT_2[64] = {
    0x00, 0x00, 0x00, 0x00, 0x1, 0x13, 0x13, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1,
    0x00, 0x00, 0x00, 0x00, 0x1, 0x05, 0x13, 0x13,
    0x00, 0x00, 0x00, 0x1, 0x13, 0x13, 0x05, 0x13,
    0x00, 0x00, 0x00, 0x1, 0x13, 0x1, 0x05, 0x08,
    0x00, 0x00, 0x00, 0x00, 0x1, 0x1, 0x05, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x05, 0x1,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x1, 0x00,
};


// Tile at index 0
static int BLANK_TILE_INDEX = 0;
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

__attribute__((section(".marge_header"))) const struct marge_header {
    char magic_number[10];
    char title[32];
    char author[32];
    char maj_version;
    char min_version;
    char rev_version;
} header = {"Marge_Sys\0", "ANIMATION TESTING ROM", "Marge Corp", 0, 2, 55};



void init_tileset()
{
    int offset = 0;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = BLANK_TILE[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = TOP_LEFT_0[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = BOT_LEFT_0[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = TOP_LEFT_1[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = BOT_LEFT_1[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = TOP_LEFT_1[i];
    }
    offset+=64;
    for(int i = 0; i < 64; i++)
    {
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = BOT_LEFT_2[i];
    }

}



void init_map()
{
    for(int i = 0; i < 64*64*2; i+=2) {
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

    // 8x16 left char
    new_addr = store_object(g_X, g_Y, TOP_LEFT_0_INDEX, 4, 0x7E04, base_addr);
    base_addr = new_addr;
    store_object(g_X+8, g_Y, TOP_LEFT_0_INDEX, 4, 0x7E05, base_addr);
}


void change_object_frame()
{
    int base_addr = OAM_ADDR;
    int new_addr;
    // Change the displayed sprite depending on animation offset
    CURRENT_ANIMATION_OFFSET = CURRENT_ANIMATION_OFFSET == MAX_ANIMATION_OFFSET ? 0 : CURRENT_ANIMATION_OFFSET + 1;
    switch(CURRENT_ANIMATION_OFFSET)
    {
        case 0:
        case 2:
            init_objects();
            break;
        case 1:
            new_addr = store_object(g_X, g_Y, TOP_LEFT_1_INDEX, 4, 0x7E04, base_addr);
            base_addr = new_addr;
            new_addr = store_object(g_X+8, g_Y, TOP_LEFT_2_INDEX, 4, 0x7E05, base_addr);
            break;
        case 3:
            new_addr = store_object(g_X, g_Y, TOP_LEFT_2_INDEX, 4, 0x7E04, base_addr);
            base_addr = new_addr;
            new_addr = store_object(g_X+8, g_Y, TOP_LEFT_1_INDEX, 4, 0x7E05, base_addr);
        default:
            break;
    }
}



int main() {
    if(!TEST) return 0;
    init_tileset();
    init_map();
    init_objects();
    remaining_frames = FRAME_PACE;

    // Setting the IME
    *(volatile unsigned char*)(IME) = 1;
    while(1)
    { }
    return 0;
}


void update_object()
{
    joypad = *(volatile unsigned char*)(JOYPAD_0); 
    if(joypad & 8)
    {   
        g_X = *(volatile unsigned char*)(OAM_ADDR);
        *(volatile unsigned char*)(OAM_ADDR) = g_X + 1;
        *(volatile unsigned char*)(OAM_ADDR+10) = g_X + 9;

        char freq_val = *(volatile unsigned char*)(CHANNEL_ADDR); 
        *(volatile unsigned char*)(CHANNEL_ADDR) = freq_val << 1;
    }
    if(joypad & 2)
    {
        g_X = *(volatile unsigned char*)(OAM_ADDR);
        *(volatile unsigned char*)(OAM_ADDR) = g_X - 1;
        *(volatile unsigned char*)(OAM_ADDR+10) = g_X + 7;

        char freq_val = *(volatile unsigned char*)(CHANNEL_ADDR); 
        *(volatile unsigned char*)(CHANNEL_ADDR) = freq_val >> 1;
    }
    if(joypad & 4)
    {
        g_Y = *(volatile unsigned char*)(OAM_ADDR + 2);
        *(volatile unsigned char*)(OAM_ADDR + 2) = g_Y + 1;
        *(volatile unsigned char*)(OAM_ADDR +12) = g_Y + 1;
    }
    if(joypad & 1)
    {
        g_Y = *(volatile unsigned char*)(OAM_ADDR + 2);
        *(volatile unsigned char*)(OAM_ADDR + 2) = g_Y - 1;
        *(volatile unsigned char*)(OAM_ADDR +12) = g_Y - 1;
    }




    if(joypad & 0xFF) {
        is_moving = 1;
    }
    else
    {
        is_moving = 0;
        init_objects();
        remaining_frames = FRAME_PACE;
    }

    if(is_moving)
    {
        remaining_frames--;
        if(remaining_frames <= 0)
        {
            change_object_frame();
            remaining_frames = FRAME_PACE;
        }
    }

    *(volatile unsigned char*)(IME) = 1;
}

// ISR
__attribute__((interrupt))
void reset_handler()
{
    main();
}

__attribute__((interrupt))
void frame_handler()
{
    update_object();
}

void update_timer()
{
    
}

__attribute__((interrupt))
void timer_handler()
{
    update_timer();
}

