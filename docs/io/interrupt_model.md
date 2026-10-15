# Interrupt Model

The R0se console has an interrupt vector table and registers to handle hardware interrupts.

!!! note

    An interrupt is an hardware *notification* that makes the CPU branches from normal flow of execution to a special handler function.

## Interrupt handlers

| Interrupt type  | Indicates |
|-----------------|-----------|
| Frame interrupt | A frame has been displayed by the console. As the console runs at 30 FPS, it occurs each ~33.33ms |
| Reset interrupt | The console has been resetted [TODO] (by the user only?) - By a joypad input |
| Timer interrupt | Depending on the values of the [timer](./timer.md) registers, an interrupt is generated at a timer tick |
|      TODO           |      TODO    |

## Registers

<u>**IME** - Interrupt Master Enable</u>

This register enable/disable the global interrupt and individual interrupt handlers.

| Bit    | 7 | 6 | 5 | 4 | 3 | 2 | 1 | O |
|--------|---|---|---|---|---|---|---|---|
| Enable | General interrupt  |   |   |   |   | Timer interrupt  | Frame interrupt  |Reset interrupt  |

<u>**IFR** - Interrupt Flag Register</u>

This register is not readable or writable by a program ([TODO] rendre ça vrai), this register is written by the console and is checked when the console checks for the interrupt branching : if a bit is set in **IFR** AND in **IME**, the program counter is set to the correct interrupt vector address.



