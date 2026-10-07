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
    _Atomic uint8_t length_r;
    /// Sweep register : Same as DMG but bit 7-4 is sweep pace, bit 3 direction, bit 2-0 is individual step
    _Atomic uint8_t sweep_r;
    /// Channel flag register : bit[7] is lenght enabled, bit[6] is sweep enabled
    _Atomic uint8_t flag_r;
    /// Volume + envelope register : bit[7-4] is the channel volume, bit[3] if set, vol inc, else dec, bit[2-0] envelope pace, if 0 -> disabled 
    _Atomic uint8_t volume_r;


    // *** Unreadable/unwritable values ***

    /// Sweep iterations, used to inc/dec frequency depending on sweep pace
    _Atomic uint8_t sweep_iterations;
    /// Envelope iterations, used to inc/dec volume depending on env pace
    _Atomic uint8_t env_iterations;
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


int apu_set_channel_volume(int channel_index);

/**
 * @brief 
 * 
 * @param set 
 * @param register 
 * @return int 
 */
int apu_set_register_enable(uint8_t set);

/**
 * @brief Updates length of the audio channels registers 
 * 
 * @param apu 
 * @return uint8_t the mask of deactivated channels 
 */
uint8_t update_apu_length(apu_t* apu);


uint8_t update_channel_length(apu_t* apu, uint8_t channel_enable_bit, audio_channel* ac);




/**
 * @brief Updates sweep of the audio channels registers 
 * 
 * @param apu 
 * @return uint8_t the mask of deactivated channels 
 */
uint8_t update_apu_sweep(apu_t* apu);


uint8_t update_apu_volume(apu_t* apu);


/**
 * @brief 
 * 
 * @param apu 
 * @param channel_enable_bit 
 * @param ac 
 * @return uint8_t The mask of the channel to disable (if sweep overflowed)
 */
uint8_t update_channel_sweep(apu_t* apu, uint8_t channel_enable_bit, audio_channel* ac);

uint8_t update_channel_volume(apu_t* apu, uint8_t channel_enable_bit, audio_channel* ac);
#endif