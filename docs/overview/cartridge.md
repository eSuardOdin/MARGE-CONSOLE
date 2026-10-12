# Cartridge

The cartridge is what is going to be loaded by the console. As seen in the [Memory Map](./memory_map.md), it is composed of a ROM and RAM section.

## ROM

The ROM section is where instructions are fetched and data is encoded. Your program should not write into this memory space. At it's core, it is an ![ELF](https://wiki.osdev.org/ELF) file and as such, you can use whatever Unix command works on them.

<figure markdown="span">
![readelf](../images/readelf.png)
<figcaption>Using a readelf command on a game ROM</figcaption>
</figure>

This section is 65408Kib long so that is what you get to store your instructions and your data such as sprites, sounds etc...

!!! note

    This may seem like a huge ROM for a *retro* system but remember all RISC-V instructions are 32-bit, there is no 8-bit or 16-bit instructions like in the NES's 6502 or GameBoy Z80.

## RAM

The cartridge has a RAM chip in order to save data, this section is 16Kib.

[TODO] Partie à étoffer