# Audio Processing Unit

The Audio Processing Unit - **APU** - is the chip that makes the console produce sound.

It is composed of some globals registers and several audio channels with special registers instructing the channel it's behavior.

## Global registers

**Audio general enable** : A global APU enable register.

| Constant name     | Register address | Purpose |
|-------------------|------------------|---------|
| `AUDIO_GEN_ENABLE`| `0x040FD400`     | Enables or disable the APU and each individual channels |
| `AUDIO_GEN_VOLUME`| `0x040FD402`     | Sets the global volume |


## Addresses

These are the channels base addresses.

| Channel | Base address | Channel type |
|---------|--------------|-----------------------|
| C0      | `0x040FD404` | Square wave channel 0 |
| C1      | `0x040FD410` | Square wave channel 1 |  
| C2      | `0x040FD41C` | Square wave channel 2 |  



## Square wave channels

These channels produces basic square sound waves, they have several attributes :

- **Frequency** : The pitch of the wave
- **Volume** : Volume of the channel
- **Envelope** : A special attribute to change volume automatically with time
- **Lenght** : The duration of the note played
- **Sweep** : A special attribute to change frequency over time, used for sound effects.
- **Flags** : A register to enable/disable some behaviors (eg: Sweep and lenght)


And here are there registers and what they do

*The registers adresses will be defined as an offset to the square wave base address*

| Register | Address offset | Purpose |
|----------|----------------|---------|
| r0       | `0`            | This register contains all the Less Sgnificant Bits of the channel frequency |
| r1       | `2`            | This register contains 5 Most Significant Bits of the channel frequency + a 3 bit approximation of the frequency decimals ([TODO] *pas utilisé*) |
| lenght_r       | `4`            | 8 bit value of the sound lenght, decreased at 32Hz |
| sweep_r       | `6`            | This register orchestrates the sweep capability of the register |
| flag_r       | `8`            | Enable/Disable some waves capabilities |
| volume_r       | `A`            | Mixes channel volume and envelope settings for the channel |

### Registers

**r0**

|Bits| [7-0]|
|----|-------|
|Purpose | Contains the 8 LSB of the 13-bits long frequency|

**r1**

|Bits| [7-5]| [4-0] |
|----|-------|------|
|Purpose | 3 bits for a decimal approximation of the frequency (1 bit = 10/7) | The 5 MSB of 13-bits long frequency |


## Noise channel