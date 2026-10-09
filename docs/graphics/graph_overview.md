# Graphics

The {{console}} fantasy console is able to display up to 32 colors. It has several layers of display that you will learn about in this section.

## Colors

Colors are the most basic graphical data structure of the console, they are represented as an index to a fixed palette hardcoded in the *"hardware"*. Every graphics asset is an array of 8-bit indexes to the color needed to be displayed. 

*The 32 colors palette :*
![color palette](../images/palette.png)

!!! note

    The values `0, 12, 4` would be displayed as `black, red, blue` pixels.

*Many thanks to [Jehkoba](https://lospec.com/palette-list/jehkoba32) for the palette.*


## Tile

A tile is an 8x8 array of pixels/color-indexes, there is up to **1024 tiles** that can live at the same time in console's tileset memory.

For instance, a tile defined as so :

``` c
static char CREEPY_GUY[64] = {
    10, 10, 10, 10, 10, 10, 10, 10, 
    10, 02, 02, 02, 02, 02, 02, 10, 
    02, 02, 02, 02, 02, 02, 02, 02, 
    02, 02, 12, 02, 02, 12, 02, 02, 
    02, 02, 02, 02, 02, 02, 02, 02, 
    02, 23, 02, 02, 02, 02, 23, 02, 
    02, 02, 23, 23, 23, 23, 02, 02, 
    02, 02, 02, 02, 02, 02, 02, 02, 
};
```

Would be displayed as so :

![tile example](../images/creepy.png)