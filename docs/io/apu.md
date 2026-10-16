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


!!! note

    If you want to encode a A4 (440Hz), you would have to put `0xB8` in this register, as 440 is `0x1B8`.

#### Frequency register 1

|Bits| [7-5]| [4-0] |
|----|-------|------|
|Purpose | Frequency decimal | Frequency MSB |

**Frequency decimal** : This 3 bits (0-7) can be used to a frequency decimal approximation (each bit = 1/7 = 1.142) *[TODO] : Je ne l'utilise pas dans la console car ce n'est pas si utile en plus de bouffer de la place, à voir ce qu'on fait de ces bits dispo*

**Frequency MSB** : Contains the 5 MSB of the 13-bit channel frequency.

!!! note

    If you want to encode a G8 (6271Hz), you would have to put `0x18` in this register, as 6271 is `0x187F`.

#### Sweep register

This register triggers a sweep sound effect on the channel frequency, what it means is just that the **frequency is decreased or increased** by a defined value **each defined APU sweep tick number**. The sweep tick is **128Hz** (so a sweep step can occur every **7.8ms**) 

|Bits    | [7-4]      | [3]       | [2-0] |
|--------|------------|-----------|-------|
|Purpose | Sweep Pace | Direction | Step  |


**Sweep Pace** : Number of APU ticks to trigger a sweep step (if you set this value to 3 for instance, a sweep step will occur every 128/3Hz)

**Direction** : If this bit is set, the sweep will increment the frequency value, otherwise, it will decrement it.

**Step** : This value is the power of 2 that will increase/decrease the frequency.

!!! note

    This is how is calculated the new frequency :

    `uint16_t new_freq = is_inc ? freq + (freq/(1<<step)) : freq - (freq/(1<<step)) ;`

 
<figure>
    <figcaption>
        <i>Sound sweep with a G5 base note :</i>
    </figcaption>
    <audio controls>
    <source src="../sound/tiding3.wav" type="audio/wav">
    Your browser does not support the audio element.
    </audio> 
</figure>

## Noise channel



## Sounds example

Here are some sounds you can produce with the console :

## Sweep 

First example is a short sweep with a G note at octave 5.

Registers : 




