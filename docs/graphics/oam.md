 # 5. Object Attribute Memory

Object Attribute Memory -**OAM**- is a memory area where the currently displayed objects are stored.

!!! warning

    The term *object*, when used in the console's context might be very far from what you know if you're familiar with OOP.

## Object

Objects are items displayed on top of the Map layer, they can be used to represent everything that has to be dynamic in a game (the player, an item, an enemy's bullet...).

### Memory representation
An **object size** in memory is `10 bytes`

An object is a basic data structure stored in memory like so *(`n` being the number of the object to store)* :

| Address | Size        | Name           | Purpose |
|---------|-------------|----------------|---------|
|`OAM_BASE_ADDRESS + n x Object size`| `16-bit`    |   `X_POS`      | The X position on screen of the object + 32 |
|`OAM_BASE_ADDRESS + n x Object size + 2`| `16-bit`    |   `Y_POS`      | The Y position on screen of the object + 32 |
|`OAM_BASE_ADDRESS + n x Object size + 4` | `16-bit`    |   `TILE INDEX`      | Index of the **top left** object tile in the [tileset](./graph_overview.md#tileset)|
|`OAM_BASE_ADDRESS + n x Object size + 6`| `16-bit`    |   `ANIM_SPRITES`      | Number of step to animate the object -if any- [TODO] : *Plus utilisé, délégué au developpeur car impose un rangement des tiles peu intuitif dans le tileset.* |
|`OAM_BASE_ADDRESS + n x Object size + 8`| `16-bit`    |   `FLAGS`      | Several 1-bit flags telling how the object's sprites will be displayed |


### Flags

The flags are switches that tells the console to display the sprites of an object a way or another. Here what they are and what they mean.

!!! note

    Naming them *flags* may be misleading as some are not individual bits, but it seemed relevent for the values to be combined as they serve the same purpose.


| Bits    | 16 15         | 14 13 12 11 10 | 9            | 8 7 6            | 5 4 3 2    | 1      | 0      |
|---------|---------------|----------------|--------------|------------------|------------|--------|--------|
| Flag    |   *unused*    | Frame duration | Transparency | Animation offset | Resolution | Y Flip | X Flip |


**Frame duration** :  If the object is animated, [TODO] : *Comme le ANIM_SPRITE, pas sûr de l'utiliser*

**Transparency** : If set, the color index `0` is transparent and does not masks the Map layer 

- *Here an exemple with **Transparency** flag set (`bit 9 == 1`) on the heart object* ![transp_set](../images/transparency_set.png)
- *Here an exemple with **Transparency** flag unset (`bit 9 == 0`) on the heart object* ![transp_set](../images/transparency_unset.png)