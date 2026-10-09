# 1. Specifications

The {{console}} fantasy console runs a custom RISC-V architecture.

It is an **8-bit architecture regarding stored data** but is a **32-bit addressable** system.
*This means you manipulate data from `$0` to `$FF` but can read/write from/to `$0 - $FFFFFFFF`*

| Component     | Description                          |
| -----------   | ------------------------------------ |
| `CPU`         | 8-bit custom RISC-V emulated CPU     |
| `Master clock`| 1.6777216 MHz                        |
| `Display`     | 240 x 160 pixels                     |