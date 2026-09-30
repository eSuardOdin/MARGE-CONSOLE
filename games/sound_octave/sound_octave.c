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

static int OCTAVE = 0x1;
static e_notes NOTE = NOTE_C;

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


void set_note(int octave, e_notes note_enum)
{
    uint16_t note = NOTE_TABLE[note_enum + 12 * octave];
    *(volatile unsigned char*)(C0R0) = note & 0xFF;
    *(volatile unsigned char*)(C0R1) = (note >> 8) & 0x1F;
}

int main() {
    init_tileset();
    init_map();
    set_note(OCTAVE, NOTE);
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

    if(BUTTONS_PRESSED & JOYPAD_RIGHT)
    {
        OCTAVE = OCTAVE == 8 ? 1 : OCTAVE + 1;
        set_note(OCTAVE,NOTE);
    }
    else if(BUTTONS_PRESSED & JOYPAD_LEFT)
    {
        OCTAVE = OCTAVE == 1 ? 8 : OCTAVE - 1;
        set_note(OCTAVE,NOTE);
    }
    if(BUTTONS_PRESSED & JOYPAD_UP)
    {
        NOTE = NOTE == NOTE_B ? NOTE_C : (e_notes)(NOTE + 1);
        if(NOTE == NOTE_C)
        {
            OCTAVE = OCTAVE == 8 ? 1 : OCTAVE + 1;
        }
        set_note(OCTAVE,NOTE);
    }
    else if(BUTTONS_PRESSED & JOYPAD_DOWN)
    {
        NOTE = NOTE == NOTE_C ? NOTE_B : (e_notes)(NOTE - 1);
        if(NOTE == NOTE_B)
        {
            OCTAVE = OCTAVE == 1 ? 8 : OCTAVE - 1;
        }
        set_note(OCTAVE,NOTE);
    }
    if(BUTTONS_PRESSED & JOYPAD_START)
    {   
        switch_map();
        uint8_t ar0 =  *(volatile unsigned char*)(AUDIO_GEN_ENABLE);
        uint8_t is_sound_enabled = ar0 & 0x1;
        if(!is_sound_enabled)
        {
            *(volatile unsigned char*)(AUDIO_GEN_ENABLE) |= 3;    
        }
        else
        {
            *(volatile unsigned char*)(AUDIO_GEN_ENABLE) &= 0b11111100;
        }
    }
   

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
