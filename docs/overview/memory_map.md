# 2. Memory Map

The {{console}} fantasy console has a 32-bit addressable memory mapped as so: 

| Begin      | End       | Description    | Notes     |
|:----------:|----------:|----------------|-----------|
| `00000000` | `03FDFFFF`|  Cartridge ROM | **Read only memory** Do **not** *read/write* it as a developer. |
| `03FE0000` | `03FFFFFF`|  Cartridge RAM | **Read/Write**, used to save data in the cartridge (16Kib available, wich means an array of 16x1024 `char` or `uint8_t`)|
| `04000000` | `04000000`|  Framebuffer   | This memory space contains indices to colors the console will display. **Do not read or write.**|
| `0404B000` | `0406AFFF`|  Console RAM   | **Read/Write** memory space. **WARNING: You should not directly interact with this space as it is attributed by the compiler to variables from your code** |
| `0406B000` |`TO DEFINE`| I/O registers  | Several registers to interact with the console, details below. |
| `0406C000` | `0407BFFF`| Tileset        | **Read/Write** - 1024 8x8 tiles are stored in this space. |
| `0407C000` | `04084000`| Maps           | **Read/Write** - Maps storage. 65536 bytes, can store 64 maps of 1024 bytes|
| `04084000` | `04FFFFFF`| UNUSED         | **Not used** - Is free to read/write until we update the console |
| `05000000` | `05000080`| Debug          | **Write** - Writing ASCII to this 128 bytes space prints in console's console. *You need to run the console locally for it to work* |




