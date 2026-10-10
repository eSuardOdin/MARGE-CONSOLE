# 4. Maps

The Map is a 512 x 512 pixel **background layer**. There is a maximum of **64 maps** that can be stored in the memory's **Maps region** *at the same time*.

## Store a Map

A Map is a contiguous array of **16-bit tile indexes** so you can think of it as **64 x 64 tiles** rather than as **512 x 512 pixels**. This means a map takes **8192 bytes** in
memory.

To store a map at the `N` index you then need to store it at `MAPS_MEMORY_REGION + (N * 8192)`.

!!! note

    Tile indexes are stored contiguously in memory by X axis first, it means **from Left to Right then Up to Down**

## Display a Map


!!! warning
    
    There is no such thing as *Map layer enable/disable* in the {{console}} so make sure your **Map index register**
    points to something **relevant and initialized in memory**, otherwise garbage would be displayed depending
    on whatever was in memory at runtime as the console <u>always display memory pointed by **Map index register**</u>.

