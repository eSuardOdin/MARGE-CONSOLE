#include "../../include/common.h"

static const int OAM_ADDR = 0x040FC000;
static const int OBJ_SIZE = 0xA;
static const int OBJ_NUMBER = 0x40;

static const int FB_ADDR = 0x04000000;
static const int IME = 0x0406B100;
static const int TILESET_ADDR = 0x0406C000;

static const int MAP_0_ADDR = 0x0407C000;
static const int MAP_0[64*64];


static const int MAP_1[64*64];
static const int MAP_1_ADDR = 0x0407E000;

static int CHANNEL_ADDR = 0x040FD404;

static int SELECTED_CHANNEL = 0x0;

static uint8_t BUTTONS_RELEASED = 0;
static uint8_t BUTTONS_PRESSED = 0;
static uint8_t BUTTONS_CURRENT = 0;
static uint8_t BUTTONS_SAVED = 0;
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

static int YELLOW_TILE_INDEX = 1;
static int YELLOW_TILE[64] = {
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14
};

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
        *(volatile unsigned char*)(TILESET_ADDR + i + offset) = YELLOW_TILE[i];
    }

}



void init_map()
{
    for(int i = 0; i < 64*64*2; i+=2) {
        *(volatile unsigned char*)(MAP_0_ADDR + i) = BLANK_TILE_INDEX & 0xFF;
        *(volatile unsigned char*)(MAP_0_ADDR + i + 1) = (BLANK_TILE_INDEX >> 8) & 0xFF;
    }
    for(int i = 0; i < 64*64*2; i+=2) {
        if(i%4) {
            *(volatile unsigned char*)(MAP_1_ADDR + i) = BLANK_TILE_INDEX & 0xFF;
            *(volatile unsigned char*)(MAP_1_ADDR + i + 1) = (BLANK_TILE_INDEX >> 8) & 0xFF;
        }
        else
        {
            *(volatile unsigned char*)(MAP_1_ADDR + i) = YELLOW_TILE_INDEX & 0xFF;
            *(volatile unsigned char*)(MAP_1_ADDR + i + 1) = (YELLOW_TILE_INDEX >> 8) & 0xFF;
        }
    }

    *(volatile unsigned char*)(MAP_INDEX) = 1;
}




int main() {
    init_tileset();
    init_map();

    // Setting the IME
    *(volatile unsigned char*)(IME) = 1;
    while(1)
    { }
    return 0;
}


void switch_map()
{
    uint8_t current_index = *(volatile unsigned char*)(MAP_INDEX);
    if(current_index)
    {
        char* error = "MAP INDEX = 0\0";
        int i = 0;
        do {
            *(volatile unsigned char*)(DEBUG_REG + i) = error[i];
        } while (error[i++] != '\0');
    }
    else
    {
        char* error = "MAP INDEX = 1\0";
        int i = 0;
        do {
            *(volatile unsigned char*)(DEBUG_REG + i) = error[i];
        } while (error[i++] != '\0');
    }
    *(volatile unsigned char*)(MAP_INDEX) = (current_index & 0x1) ? 0 : 1;
}


void update_keys()
{
    // Get current state
    BUTTONS_CURRENT     = *(volatile unsigned char*)(JOYPAD_0);
    BUTTONS_PRESSED     = BUTTONS_CURRENT & ~BUTTONS_SAVED;
    BUTTONS_RELEASED    = BUTTONS_SAVED & ~BUTTONS_CURRENT;

    if(BUTTONS_PRESSED & JOYPAD_START)
    {   
        switch_map();
        uint8_t ar0 =  *(volatile unsigned char*)(AUDIO_GEN_ENABLE);
        uint8_t is_sound_enabled = ar0 & 0x1;
        if(!is_sound_enabled)
        {
            *(volatile unsigned char*)(AUDIO_GEN_ENABLE) |= 1;    
        }
        else
        {
            *(volatile unsigned char*)(AUDIO_GEN_ENABLE) &= 0b11111110;
        }
    }
    if(BUTTONS_PRESSED & JOYPAD_RIGHT)
    {
        SELECTED_CHANNEL = SELECTED_CHANNEL == 2 ? 0 : SELECTED_CHANNEL + 1;
    }
    if(BUTTONS_PRESSED & JOYPAD_LEFT)
    {
        SELECTED_CHANNEL = SELECTED_CHANNEL ? SELECTED_CHANNEL - 1 : 2;
    }
    if(BUTTONS_PRESSED & JOYPAD_SELECT)
    {   
        uint8_t ar0 = *(volatile unsigned char*)(AUDIO_GEN_ENABLE);
        switch(SELECTED_CHANNEL)
        {
            case 0:
                if(!(ar0 & 0x2))
                {
                    *(volatile unsigned char*)(AUDIO_GEN_ENABLE) = ar0 | 0x2;  
                }
                else
                {
                    *(volatile unsigned char*)(AUDIO_GEN_ENABLE) = ar0 & 0xFD;
                }
                break;
            case 1:
                if(!(ar0 & 0x4))
                {
                    *(volatile unsigned char*)(AUDIO_GEN_ENABLE) = ar0 | 0x4;  
                }
                else
                {
                    *(volatile unsigned char*)(AUDIO_GEN_ENABLE) = ar0 & 0xFB;
                }
                break;
            case 2:
                if(!(ar0 & 0x8))
                {
                    *(volatile unsigned char*)(AUDIO_GEN_ENABLE) = ar0 | 0x8;  
                }
                else
                {
                    *(volatile unsigned char*)(AUDIO_GEN_ENABLE) = ar0 & 0xF7;
                }
                break;
        }
    }

    // if(joypad & 0xFF) {
    //     is_moving = 1;
    //     char current_sound_enable = *(volatile unsigned char*)(0x040FD400);
    //     *(volatile unsigned char*)(0x040FD400) = 1;
    // }
    // else
    // {
    //     is_moving = 0;
    //     init_objects();
    //     remaining_frames = FRAME_PACE;
    // }
    BUTTONS_SAVED = BUTTONS_CURRENT;
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
    update_keys();
}
