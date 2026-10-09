# 2. Memory Map



| Begin       | End        | Description    | Notes     |
|:-----------:|-----------:|----------------|-----------|
|  `00000000` | `03FDFFFF` |  Cartridge ROM | **Read only memory** Do **not** *read/write* it as a developer. |
|  `03FE0000` | `03FFFFFF` |  Cartridge RAM | **Read/Write memory**, used to save data in the cartridge (16Kib available, wich means an array of 16x1024 `char` or `uint8_t`)|