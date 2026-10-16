# Audio Processing Unit

The Audio Processing Unit - **APU** - is the chip that makes the console produce sound.

It is composed of some globals registers and several audio channels with special registers instructing the channel it's behavior.

The APU encodes its frequency on a **13-bit word** and as so can produce sounds from **1Hz-8191Hz** *([TODO] : Checker quelle est la plus basse fréquence audible et agréable en onde carré pour gagner de la place sur les basses avec un offset de la fréquence )*


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

| Register                   | Address offset | Purpose |
|----------------------------|----------------|---------|
| Frequency register 0       | `0`            | This register contains all the Less Significant Bits of the channel frequency |
| Frequency register 1       | `2`            | This register contains 5 Most Significant Bits of the channel frequency + a 3 bit approximation of the frequency decimals ([TODO] *pas utilisé*) |
| Length register            | `4`            | 8 bit value of the sound lenght, decreased at 32Hz |
| Sweep register             | `6`            | This register orchestrates the sweep capability of the register |
| Flag register              | `8`            | Enable/Disable some waves capabilities |
| Volume register            | `A`            | Mixes channel volume and envelope settings for the channel |

### Registers

#### Frequency register 0

|Bits| [7-0]|
|----|-------|
|Purpose | Frequency LSB |

**Frequency LSB** : Constains the 8 Less Significants Bits of the 13-bit channel frequency.

$$
\cos x=\sum_{k=0}^{\infty}\frac{(-1)^k}{(2k)!}x^{2k}
$$

!!! note

    If you want to encode a A4 (440Hz), you would have to put `0xB8` in this register, as 440 is 1B8_{16}.

#### Frequency register 1

|Bits| [7-5]| [4-0] |
|----|-------|------|
|Purpose | Frequency decimal | The 5 MSB of 13-bits long frequency |

**Frequency decimal** : This 3 bits (0-7) can be used to a frequency decimal approximation (each bit = 1/7 = 1.142) *[TODO] : Je ne l'utilise pas dans la console car ce n'est pas si utile en plus de bouffer de la place, à voir ce qu'on fait de ces bits dispo*

**sweep_r**

|Bits| [7-5]| [4-0] |
|----|-------|------|
|Purpose | 3 bits for a decimal approximation of the frequency (1 bit = 10/7) | The 5 MSB of 13-bits long frequency |

## Noise channel



## Sounds example

Here are some sounds you can produce with the console :

## Sweep 

First example is a short sweep with a G note at octave 5.

Registers : 


<figure>
    <figcaption>
        <i>Sound sweep with a G5 base note :</i>
    </figcaption>
    <audio controls>
    <source src="../sound/tiding3.wav" type="audio/wav">
    Your browser does not support the audio element.
    </audio> 
</figure>

