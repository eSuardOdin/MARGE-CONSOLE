# 4. Maps

The Map is a 512 x 512 pixel **background layer**. There is a maximum of **64 maps** that can be stored in the memory's **Maps region** *at the same time*.

## Store a Map

A Map is a contiguous array of **16-bit tile indexes** so you can think of it as **64 x 64 tiles** rather than as **512 x 512 pixels**. This means a map takes **8192 bytes** in
memory.

To store a map at the `N` index you then need to store it at `MAPS_MEMORY_REGION + (N * 8192)`, make sure you are not storing it shifted in memory.

!!! note

    Tile indexes are stored contiguously in memory by X axis first, it means **from Left to Right then Up to Down** - Indexes 0-63 are the first map line (Y=0), 64-127 are the second map line (Y=1).

## Display a Map

There is several **system registers** that affects what the console will display regarding the map layer :

### Map index register

| Register    | Memory address | Purpose |
|-------------|----------------|---------|
| `MAP_INDEX` |   `0406B002`   | Tells the console to display the map located at `MAPS_MEMORY_REGION + (MAP_INDEX * 8192)` |

*It means that you need to write `2` at memory address `0406B002` to display the third map you stored in memory.*

[TODO] : *Si le register est plus grand que 64, la console va afficher du garbage en allant lire dans la région mémoire après la map region, choisir si on wrap ou laisse faire* 

!!! warning
    
    There is no such thing as *Map layer enable/disable* in the {{console}} so make sure your `MAP_INDEX` register
    points to something **relevant and initialized in memory**, otherwise garbage would be displayed depending
    on whatever was in memory at runtime as the console <u>**always** displays memory pointed by `MAP_INDEX` register</u>.

### Scroll registers

Maps are 512 x 512 pixels, as the console display is 240 x 160 pixels, only a portion of it will be displayed. There is two registers 
that change the portion of the map that will be displayed.

| Register    | Memory address | Purpose |
|-------------|----------------|---------|
| `SCROLL_X`  |   `0406B004`   | Shifts the X of displayed part of the map by the number of pixels written to this register.  |
| `SCROLL_Y`  |   `0406B006`   | Shifts the Y of displayed part of the map by the number of pixels written to this register.|

[TODO] : *ajouter une image + se poser la question du wrapping et du 512x512 vs le register 8-bit, augmentation de la taille du registre ?*




