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
    c0->channel_sound = c0_sound;
    apu->c0 = c0;
    apu_set_channel_freq(0);

    // Init CHANNEL 1
    audio_channel *c1 = malloc(sizeof(audio_channel));
    atomic_init(&c1->r0, 0x15);
    atomic_init(&c1->r1, 1);
    c1->channel_sound = c1_sound;
    apu->c1 = c1;
    apu_set_channel_freq(1);

    // Init CHANNEL 2
    audio_channel *c2 = malloc(sizeof(audio_channel));
    atomic_init(&c2->r0, 0x4A);
    atomic_init(&c2->r1, 1);
    c2->channel_sound = c2_sound;
    apu->c2 = c2;
    apu_set_channel_freq(2);
    return apu;
}



int apu_set_channel_freq(int channel_index)
{
    uint8_t msb = (atomic_load(&apu->c0->r1)) & 0x1F;
    uint8_t div = atomic_load(&apu->c0->r1) & 0xE0 >> 5; 
    double freq = (double)atomic_load(&apu->c0->r0) + (double)(msb << 8);
    switch(channel_index)
    {
        case 0:
            ma_waveform_set_frequency((ma_waveform*)c0_sound->pDataSource, freq);
            break;
        case 1:
            ma_waveform_set_frequency((ma_waveform*)c1_sound->pDataSource, freq);
            break;
        case 2:
            ma_waveform_set_frequency((ma_waveform*)c2_sound->pDataSource, freq);
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