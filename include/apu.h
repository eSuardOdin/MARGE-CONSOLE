#ifndef APU_H
#define APU_H

#include "common.h"
#include <stdatomic.h>
#include "miniaudio.h"
/**
 * @file apu.h
 * @brief Audio chip handling console's audio.
 */

#define APU_FLAG_ON     0x1


/**
 * @brief Struct representing an audio channel and it's registers
 * 
 */
typedef struct {
    /// LSB 8 bits of the frequency
    _Atomic uint8_t r0;
    /// bits[7-5] 3 decimal approximation of frequency, bits[4-0] MSB 5 bits of frequency 
    _Atomic uint8_t r1;
    /// Lenght register
    _Atomic uint8_t r2;
    /// Sweep register
    _Atomic uint8_t sweep_r;
    /// Sound 
    ma_sound* channel_sound;

} audio_channel;

/**
 * @brief Represents the chip responsible for the audio playing of the console
 * 
 */
typedef struct {
    /// General enable register, bit 0 is a general enable, bit[6-1] are enable flags for channels C0-C5 
    _Atomic uint8_t ar0;
    /// General audio volume setting, bit[7-4] handle the left volume, bit[3-0] handle the right volume 
    _Atomic uint8_t ar1;
    /// First square wave audio channel
    audio_channel* c0;
    /// Second square wave audio channel
    audio_channel* c1;
    /// Second square wave audio channel
    audio_channel* c2;
} apu_t;


/**
 * @brief Inits the APU, instanciate all the relevant structs for miniaudio to work (engine, channels sound),
 * links the callback to the device used by APU.
 * 
 * @return apu_t* Pointer to the apu to initialized
 */
apu_t* init_apu();

/**
 * @brief Sets the new frequency of a channel (uses global variables in apu.c), this function is
 * called in bus.c
 * 
 * @param channel_index Index of the channel
 * @return int Error code
 */
int apu_set_channel_freq(int channel_index);

/**
 * @brief 
 * 
 * @param set 
 * @param register 
 * @return int 
 */
int apu_set_register_enable(uint8_t set);
#endif