# MARGE-CONSOLE


# To do
- Some values stored in console's memory need more than 8 bit, define what to do with them: May need to implement functions in the custom lib to access/write these values (GetScrollX() -> Get the 16bit scrollX value)
- 



## Testing the CPU

### R TYPE

ADD: 
![alt text](./img/add.png)
Test:
- `ADD x1(1), x1(1), x2($FFFFFFFF) => x1 = -1`
```
x1:= 1
x2:= $FFFFFFFF
0000 0000 0010 0000 1000 0000 1011 0011
00        20        80        B3
B3 80 20 00
```

- `ADD x1($80000000), x1($80000000), x2(1) => x1 = Max negative value`
```
x1:= $80000000
x2:= 1
0000 0000 0010 0000 1000 0000 1011 0011
00        20        80        B3
B3 80 20 00
```

## Memory Map

The 32 bit architecture is able to adress ~4.1Gib

### [0x00000000 - 0x03FFFFFF]: Cartridge memory 

**~64Mib** Reading/Writing on this space will interact with the cartridge
 
- `[0x00000000 - 0x03FDFFFF] ROM Memory` This adress space is readonly, it is where all programs instructions are stored with game data (sprites, audio...) 
- `[0x03FE0000 - 0x03FFFFFF] RAM Memory` This ~128Kib adress space is for embedded RAM into the cartridge. This space can be readen from / written to in order to load/save data

### [0x04000000 - 0x0404AFFF]: Video adress space (to extend)

- `[0x04000000 - 0x0404AFFF] Framebuffer` Contains pixels to be displayed on the 240x160px display. Data is encoded as 1 byte per pixel ([5:0]: index to one of the 32 colors in the palette)

### [0x404B000 - 0x406AFFF] RAM

### [0x406B000 - ] IO Adresses
#### Controllers
Controller state is stored on a single byte, bit 6 and 7 are selectors in order to know what we want to read and the other six bits are state of the observed buttons.

**Not sure yet how to handle the writing** :
*There my be no need to store state when it change, i can just poll the state when asked by the ROM*
 
*Exemple with states of bits [6-7]:*
- `00 - Bit 0 to 5 represents respectively: Right, Left, Up, Down, Select, Start`
- `01 - Bit 0 to 5 represents respectively: A, B, X, Y, R trigger, L trigger`
- `10 - Bit 0 to 5 represents respectively: TBD`
- `11 - Bit 0 to 5 represents respectively: TBD`

`[0x406B000] First controller` 

## To think about

- How to handle cartridge memory: allocating 64Mib for the program lifetime seems a lot when cartridge may be only 1Mib or less


## Palette

![alt text](./img/palette.png)