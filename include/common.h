
#ifndef COMMON_H
#define COMMON_H



#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <elf.h>
#include <math.h>

/**
 * @file common.h
 * @brief Common constants and data structures. Helps documenting memory map and common structures (like marge_header)
 */

#define SCREEN_WIDTH        240
#define SCREEN_HEIGHT       160
#define SCALE               3


// --- CPU SPECS ---
#define FREQUENCY_MHZ       16777216
#define TIMER_FREQUENCY_MHZ 16384
#define APU_FREQUENCY_MHZ   32
#define APU_SWEEP_FREQ      128
#define APU_ENV_FREQ        64
#define INSTRUCTION_COST    1
#define FPS_TARGET          30
#define MS_TARGET           33 // 1/30 seconds as ms
#define INST_PER_FRAME      (FREQUENCY_MHZ / INSTRUCTION_COST / FPS_TARGET)

// --- MEMORY MAPPING OFFSETS AND STARTING POINT ---
#define CART_RAM_OFST       0x03FE0000
#define VRAM_OFST           0x04000000
#define FB_OFST             0x04000000
#define RAM_OFST            0x0404B000
#define IO_OFST             0x0406B000
#define TILESET_OFST        0x0406C000
#define MAPS_OFST           0x0407C000

#define OAM_OFST            0x040FC000 // Size : 0x1400
#define AUDIO_OFST          0x040FD400

#define DEBUG_REG           0x05000000 // Size : (128 bytes)
#define DEBUG_REG_END       0x05000080

#define STACK_OFST          0x05FF8000
#define STACK_END           0x06000000 // 32Kb


// --- JOYPAD ---
// Joypad address
#define JOYPAD_0            0x0406B000
// Define joypad masks
#define JOYPAD_UP           0x1 
#define JOYPAD_LEFT         0x2
#define JOYPAD_DOWN         0x4 
#define JOYPAD_RIGHT        0x8
#define JOYPAD_A            0x10
#define JOYPAD_B            0x20
#define JOYPAD_START        0x40
#define JOYPAD_SELECT       0x80

// --- System Registers ---
// Defines wich map is to be printed
#define MAP_INDEX           0x0406B002
#define SCROLL_X            0x0406B004 // Scroll of BG (X OFFSET)
#define SCROLL_Y            0x0406B006 // Scroll of BG (Y OFFSET)
#define FRAME_COUNTER       0x0406B0F0
#define TIMER_ENABLE        0x0406B0F2
#define TIMER               0x0406B0F4
#define TIMER_MOD           0x0406B0F6
#define DIV_COUNTER         0x0406B0F8
#define INTERRUPT_REGISTER  0x0406B100
#define INTERRUPT_FLAGS     0x0406B102
#define DEBUG_REGISTER      0x0406B104
// Interrupt flags
#define IRQ_RESET_F         0x1
#define IRQ_FRAME_F         0x2
#define IRQ_TIMER_F         0x4
// Interrupt Vectors - Not accurate
// #define ISR_RESET           0x00
// #define ISR_FRAME           0x40
// #define ISR_TIMER           0x80

// --- Audio ---
#define AUDIO_GEN_ENABLE    0x040FD400
#define AUDIO_GEN_VOLUME    0x040FD402

#define C0R0                0x040FD404  // LSB
#define C0R1                0x040FD406  // MSB + DECIMAL
#define C0R2                0x040FD408  // LEN REGISTER
#define C0R3                0x040FD40A  // SWEEP REGISTER
#define C0R4                0x040FD40C  // FLAG REGISTER
#define C0R5                0x040FD40E  // SOUND REGISTER

#define C1R0                0x040FD410 // LSB 
#define C1R1                0x040FD412 // MSB + DECIMAL
#define C1R2                0x040FD414 // LEN REGISTER
#define C1R3                0x040FD416 // SWEEP REGISTER
#define C1R4                0x040FD418 // FLAG REGISTER
#define C1R5                0x040FD41A // SOUND REGISTER

#define C2R0                0x040FD41C // LSB
#define C2R1                0x040FD41E // MSB + DECIMAL
#define C2R2                0x040FD420 // LEN REGISTER
#define C2R3                0x040FD422 // SWEEP REGISTER
#define C2R4                0x040FD424 // FLAG REGISTER
#define C2R5                0x040FD426 // SOUND REGISTER


// --- Utility ---
#define MAP_BYTES           0x80000   
#define TILE_SIZE           0x40
#define TRUE                1
#define FALSE               0
extern const int COLORSPAL[32];

// --- Music ---                 0.142   0.284 0.426  0.568 0.710  0.852  1~
// Notes (Max: 0x1FFF - 8191) - 001=20 010=40 011=60 100=80 101=A0 110=C0 111=E0
typedef enum {
    NOTE_C = 0, NOTE_Cs, NOTE_D, NOTE_Ds, NOTE_E, NOTE_F,
    NOTE_Fs, NOTE_G, NOTE_Gs, NOTE_A, NOTE_As, NOTE_B
}e_notes;

static const uint16_t NOTE_TABLE[108] = {
    //  C       C#      D       D#      E       F       F#      G       G#      A       A#      B
    0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0016, 0x0017, 0x0018, 0x001A, 0x001B, 0x001D, 0x001E, // Octave 0
    0x0021, 0x0023, 0x0025, 0x0027, 0x0029, 0x002C, 0x002E, 0x0031, 0x0034, 0x0037, 0x003A, 0x003D, // Octave 1
    0x0041, 0x0045, 0x0049, 0x004E, 0x0052, 0x0057, 0x005C, 0x0062, 0x0068, 0x006E, 0x0074, 0x007B, // Octave 2
    0x0083, 0x008A, 0x0093, 0x009C, 0x00A5, 0x00AF, 0x00B9, 0x00C4, 0x00D0, 0x00DC, 0x00E9, 0x00F7, // Octave 3
    0x0105, 0x0115, 0x0126, 0x0137, 0x014A, 0x015D, 0x0172, 0x0188, 0x019F, 0x01B8, 0x01D2, 0x01EE, // Octave 4
    0x020B, 0x022A, 0x024B, 0x026E, 0x0293, 0x02BA, 0x02E4, 0x0310, 0x033F, 0x0370, 0x03A4, 0x03DC, // Octave 5
    0x0416, 0x0455, 0x0496, 0x04DD, 0x0526, 0x0575, 0x05C8, 0x0620, 0x067D, 0x06E0, 0x073E, 0x07B7, // Octave 6
    0x082D, 0x08A9, 0x092D, 0x09B9, 0x0A4D, 0x0AE9, 0x0B8F, 0x0C40, 0x0CFA, 0x0DC0, 0x0E91, 0x0F6F, // Octave 7
    0x105A, 0x1153, 0x125A, 0x1372, 0x149A, 0x15D5, 0x171F, 0x187F, 0x19F4, 0x1B80, 0x1D22, 0x1EDE  // Octave 8
};

static const int TIMER_FREQUENCIES[7] = {
    65536, 32768, 16384, 8192, 4096, 2048, 1024
};

typedef struct {
    /// Magic number used to ensure the ROM is ok. Needs to be "Marge_Sys\0" (null terminated important)
    char magic_number[10];
    /// ASCII encoded 32 char title
    char title[32];
    /// ASCII encoded 32 char authors
    char author[32];
    /// Major version identifier 0-255
    char maj_version;
    /// Minor version identifier 0-255
    char min_version;
    /// Revision version identifier 0-255
    char rev_version;
} marge_header;




#endif