#include "apu.h"
#include "common.h"
#define DEVICE_FORMAT               ma_format_f32
#define DEVICE_CHANNELS             2
#define DEVICE_SAMPLE_RATE          48000

// Globals to use in callback - Warning, verify if something else is better.
ma_sound* c0_sound;
ma_sound* c1_sound;
ma_sound* c2_sound;
apu_t* apu;
void data_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count)
{
    if(atomic_load(&apu->ar0) & APU_FLAG_ON)
    {
        ma_engine_read_pcm_frames((ma_engine*)device->pUserData, output, frame_count, NULL);
    }
}

apu_t* init_apu()
{
    apu = malloc(sizeof(apu_t));
    c0_sound = malloc(sizeof(ma_sound));
    c1_sound = malloc(sizeof(ma_sound));
    c2_sound = malloc(sizeof(ma_sound));
    // Miniaudio init
    ma_result result;
    // Engine
    ma_engine_config engine_config;
    ma_engine* engine = malloc(sizeof(ma_engine));
    // Device
    ma_device_config device_config;
    ma_device* device = malloc(sizeof(ma_device));

    // Device config
    device_config                   = ma_device_config_init(ma_device_type_playback);
    device_config.playback.format   = DEVICE_FORMAT;
    device_config.playback.channels = DEVICE_CHANNELS;
    device_config.sampleRate        = DEVICE_SAMPLE_RATE;
    device_config.dataCallback      = data_callback;
    device_config.pUserData         = engine;

    // Init the device
    result = ma_device_init(NULL, &device_config, device);
    if(result != MA_SUCCESS)
    {
        fprintf(stderr, "init_apu() - Error initializing the device: %d\n", result);
        return NULL;
    }
    // Init the engine
    engine_config               = ma_engine_config_init();
    engine_config.pDevice       = device;
    
    result = ma_engine_init(&engine_config, engine);
    if(result != MA_SUCCESS)
    {
        fprintf(stderr, "init_apu() - Error initializing the engine: %d\n", result);
        return NULL;
    }
    
    // Init C0 waveform datasource
    ma_waveform* square_wave_c0 = malloc(sizeof(ma_waveform));
    ma_waveform_config square_wave_c0_config;
    square_wave_c0_config = ma_waveform_config_init(device->playback.format, device->playback.channels, device->sampleRate, ma_waveform_type_square, 0.025, 220);
    result = ma_waveform_init(&square_wave_c0_config, square_wave_c0);
    if(result != MA_SUCCESS)
    {
        fprintf(stderr, "init_apu() - Error initializing the sound 0 square wave: %d\n", result);
        return NULL;
    }
    
    result = ma_sound_init_from_data_source(engine, square_wave_c0, 0, NULL, c0_sound);
    if(result != MA_SUCCESS)
    {
        fprintf(stderr, "init_apu() - Error initializing the sound for channel 0: %d\n", result);
        return NULL;
    }
    
    // Init C1 waveform datasource
    ma_waveform* square_wave_c1 = malloc(sizeof(ma_waveform));
    ma_waveform_config square_wave_c1_config;
    square_wave_c1_config = ma_waveform_config_init(device->playback.format, device->playback.channels, device->sampleRate, ma_waveform_type_square, 0.025, 277);
    result = ma_waveform_init(&square_wave_c1_config, square_wave_c1);
    if(result != MA_SUCCESS)
    {
        fprintf(stderr, "init_apu() - Error initializing the sound 1 square wave: %d\n", result);
        return NULL;
    }
    
    result = ma_sound_init_from_data_source(engine, square_wave_c1, 0, NULL, c1_sound);
    if(result != MA_SUCCESS)
    {
        fprintf(stderr, "init_apu() - Error initializing the sound for channel 1: %d\n", result);
        return NULL;
    }
    

    // Init C2 waveform datasource
    ma_waveform* square_wave_c2 = malloc(sizeof(ma_waveform));
    ma_waveform_config square_wave_c2_config;
    square_wave_c2_config = ma_waveform_config_init(device->playback.format, device->playback.channels, device->sampleRate, ma_waveform_type_square, 0.025, 330);
    result = ma_waveform_init(&square_wave_c2_config, square_wave_c2);
    if(result != MA_SUCCESS)
    {
        fprintf(stderr, "init_apu() - Error initializing the sound 2 square wave: %d\n", result);
        return NULL;
    }
    
    result = ma_sound_init_from_data_source(engine, square_wave_c2, 0, NULL, c2_sound);
    if(result != MA_SUCCESS)
    {
        fprintf(stderr, "init_apu() - Error initializing the sound for channel 2: %d\n", result);
        return NULL;
    }

    ma_sound_start(c0_sound);
    ma_sound_start(c1_sound);
    ma_sound_start(c2_sound);


    // Init GLOBAL REGISTERS
    atomic_init(&apu->ar0, 0);
    atomic_init(&apu->ar1, 0);
    
    // Init CHANNEL 0
    audio_channel *c0 = malloc(sizeof(audio_channel));
    atomic_init(&c0->r0, 0xB8);
    atomic_init(&c0->r1, 1);
    atomic_init(&c0->length_r, 0);
    atomic_init(&c0->sweep_r, 0);
    atomic_init(&c0->flag_r, 0);
    atomic_init(&c0->volume_r, 0);
    atomic_init(&c0->sweep_iterations, 0);
    atomic_init(&c0->env_iterations, 0);
    c0->channel_sound = c0_sound;
    apu->c0 = c0;
    apu_set_channel_freq(0);

    // Init CHANNEL 1
    audio_channel *c1 = malloc(sizeof(audio_channel));
    atomic_init(&c1->r0, 0x15);
    atomic_init(&c1->r1, 1);
    atomic_init(&c1->length_r, 0);
    atomic_init(&c1->sweep_r, 0);
    atomic_init(&c1->flag_r, 0);
    atomic_init(&c1->volume_r, 0);
    atomic_init(&c1->sweep_iterations, 0);
    atomic_init(&c1->env_iterations, 0);
    c1->channel_sound = c1_sound;
    apu->c1 = c1;
    apu_set_channel_freq(1);

    // Init CHANNEL 2
    audio_channel *c2 = malloc(sizeof(audio_channel));
    atomic_init(&c2->r0, 0x4A);
    atomic_init(&c2->r1, 1);
    atomic_init(&c2->length_r, 0);
    atomic_init(&c2->sweep_r, 0);
    atomic_init(&c2->flag_r, 0);
    atomic_init(&c2->volume_r, 0);
    atomic_init(&c2->sweep_iterations, 0);
    atomic_init(&c2->env_iterations, 0);
    c2->channel_sound = c2_sound;
    apu->c2 = c2;
    apu_set_channel_freq(2);
    return apu;
}



int apu_set_channel_freq(int channel_index)
{
    uint8_t msb;
    uint8_t div;
    double freq;
    switch(channel_index)
    {
        case 0:
            msb = (atomic_load(&apu->c0->r1)) & 0x1F;
            div = atomic_load(&apu->c0->r1) & 0xE0 >> 5; 
            freq = (double)atomic_load(&apu->c0->r0) + (double)(msb << 8);
            ma_waveform_set_frequency((ma_waveform*)c0_sound->pDataSource, freq);
            break;
        case 1:
            msb = (atomic_load(&apu->c1->r1)) & 0x1F;
            div = atomic_load(&apu->c1->r1) & 0xE0 >> 5; 
            freq = (double)atomic_load(&apu->c1->r0) + (double)(msb << 8);
            ma_waveform_set_frequency((ma_waveform*)c1_sound->pDataSource, freq);
            break;
        case 2:
            msb = (atomic_load(&apu->c2->r1)) & 0x1F;
            div = atomic_load(&apu->c2->r1) & 0xE0 >> 5; 
            freq = (double)atomic_load(&apu->c2->r0) + (double)(msb << 8);
            ma_waveform_set_frequency((ma_waveform*)c2_sound->pDataSource, freq);
            break;
    }
    return 0;
}


int apu_set_channel_volume(int channel_index)
{
    float vol;
    switch(channel_index)
    {
        case 0:
            vol = (float)(((atomic_load(&apu->c0->volume_r)) & 0xF0) >> 4) / 15.0;
            ma_sound_set_volume(c0_sound, vol);
            break;
        case 1:
            vol = (float)(((atomic_load(&apu->c1->volume_r)) & 0xF0) >> 4) / 15.0;
            ma_sound_set_volume(c1_sound, vol);
            break;
        case 2:
            vol = (float)(((atomic_load(&apu->c2->volume_r)) & 0xF0) >> 4) / 15.0;
            ma_sound_set_volume(c2_sound, vol);
            break;
    }
    return 0;
}

int apu_set_register_enable(uint8_t mask)
{
    for(int i = 0; i < 8; i++)
    {

        switch(i)
        {
            case 1:
                if(mask & 1 << i)
                    ma_sound_start(c0_sound);
                else
                    ma_sound_stop(c0_sound);
                break;
            case 2:
                if(mask & 1 << i)
                    ma_sound_start(c1_sound);
                else
                    ma_sound_stop(c1_sound);
                break;
            case 3:
                if(mask & 1 << i)
                    ma_sound_start(c2_sound);
                else
                    ma_sound_stop(c2_sound);
                break;
        }
    }
    return 0;
}



uint8_t update_apu_length(apu_t* apu)
{
    uint8_t result = 0;

    result |= update_channel_length(apu, 0x2, apu->c0);
    result |= update_channel_length(apu, 0x4, apu->c1);
    result |= update_channel_length(apu, 0x8, apu->c2);
    return result;
}

uint8_t update_channel_length(apu_t* apu, uint8_t channel_enable_bit, audio_channel* ac)
{
    if(ac->flag_r & 0x80) // If lenght enabled
    {
        // If overflow, shut down channel
        if(ac->length_r == 0xFF)
        {
            ac->length_r = 0;
            ac->flag_r &= ~(0x80);
            apu->ar0 &= ~(channel_enable_bit);
            return channel_enable_bit;
        }
        else
        {
            ac->length_r++;
            return 0;
        }
    }
    return 0;
}





uint8_t update_apu_sweep(apu_t* apu)
{
    uint8_t result = 0;
    // May need to change returned value, was used as a shutdown mask when sweeping was over.
    result |= update_channel_sweep(apu, 0x2, apu->c0);
    result |= update_channel_sweep(apu, 0x4, apu->c1);
    result |= update_channel_sweep(apu, 0x8, apu->c2);
    return result;
}




uint8_t update_channel_sweep(apu_t* apu, uint8_t channel_enable_bit, audio_channel* ac)
{
    uint8_t flags = atomic_load(&ac->flag_r);
    if(flags & 0x40)
    {
        uint8_t iter = atomic_load(&ac->sweep_iterations);
        uint8_t sweep_r = atomic_load(&ac->sweep_r);
        uint8_t pace = (sweep_r & 0xF0) >> 4;
        uint8_t step = (sweep_r & 0x7) >> 4;
        char is_inc = sweep_r & 0x8;
        uint8_t msb = (atomic_load(&ac->r1)) & 0x1F;
        uint8_t lsb = atomic_load(&ac->r0);
        uint16_t freq = (msb << 8) | lsb;

        // Inc/Dec frequency
        if(iter == pace)
        {
            uint16_t new_freq = is_inc ? freq + (freq/(1<<step)) : freq - (freq/(1<<step)) ;
            atomic_store(&ac->sweep_iterations, 0);
            // If new freq overflows 7FF, the sweep is disabled and channel too
            if(new_freq > 0x7FF)
            {
                flags &= ~(0x40);
                atomic_store(&ac->flag_r, flags);
            }
            // Else, set new freq
            else
            {
                atomic_store(&ac->r1, (new_freq & 0x1F00) >> 8);
                atomic_store(&ac->r0, (new_freq & 0xFF));
                switch(channel_enable_bit)
                {
                    case 0x2:
                        ma_waveform_set_frequency((ma_waveform*)c0_sound->pDataSource, (double)new_freq);
                        break;
                    case 0x4:
                        ma_waveform_set_frequency((ma_waveform*)c1_sound->pDataSource, (double)new_freq);
                        break;
                    case 0x8:
                        ma_waveform_set_frequency((ma_waveform*)c2_sound->pDataSource, (double)new_freq);
                        break;
                }
            }
        }
        else
        {
            iter++;
            atomic_store(&ac->sweep_iterations, iter);
        }
        return 0;
    }
}


uint8_t update_apu_volume(apu_t* apu)
{
    uint8_t result = 0;
    uint8_t enabled_register = atomic_load(&apu->ar0);
    if(enabled_register & 0x1)
    {
        if(enabled_register & 0x2)
            result |= update_channel_volume(apu, 0x2, apu->c0);
        if(enabled_register & 0x4)
            result |= update_channel_volume(apu, 0x4, apu->c1);
        if(enabled_register & 0x8)
            result |= update_channel_volume(apu, 0x8, apu->c2);
    }
    return result;
}



uint8_t update_channel_volume(apu_t* apu, uint8_t channel_enable_bit, audio_channel* ac)
{
    uint8_t vol_reg = atomic_load(&ac->volume_r);
    if(vol_reg & 0x7) // If enveloppe is enabled
    {
        uint8_t iter = atomic_load(&ac->env_iterations);
        if(iter == (vol_reg & 0x7))
        {
            iter = 0;
            uint8_t current_vol = (atomic_load(&apu->c0->volume_r) & 0xF0) >> 4;
            if(vol_reg & 0x8 && current_vol < 0xF) // If bit[3] is set, volume increases
            {
                current_vol++;
            }
            else if(!(vol_reg & 0x8) && current_vol)
            {
                current_vol--;
            }
            atomic_store(&ac->volume_r, (current_vol << 4) | vol_reg & 0x0F);
            apu_set_channel_volume(((uint8_t)log2(channel_enable_bit)) - 1);
        }
        else
        {
            iter++;
        }

        atomic_store(&ac->env_iterations, iter);


    }

    return 0;
}

