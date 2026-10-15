# Gamepad

R0se's gamepad is a simple old school game pad, it contains only 8 buttons.

It fits into a 8 bit register and each bit can be tested against a mask to know wich keys are being pressed.

The bit corresponding to a button is pressed if it's value is `1`, if it is `0`, the button is released.

**Gamepad** register address is `0x0406B000`. Here is the register :

|Bit   | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|------|---|---|---|---|---|---|---|---|
|Button| Up|Left|Down|Right|A|B|Start|Select|


|  Key        | Flag name      | Flag   |
|-------------|----------------|--------|
| Up arrow    | `JOYPAD_UP`    | `0x1`  |
| Left arrow  | `JOYPAD_LEFT`  | `0x2`  |
| Down arrow  | `JOYPAD_DOWN`  | `0x4`  |
| Right arrow | `JOYPAD_RIGHT` | `0x8`  |
| A           | `JOYPAD_A`     | `0x10` |
| B           | `JOYPAD_B`     | `0x20` |
| Start       | `JOYPAD_START` | `0x40` |
| Select      | `JOYPAD_SELECT`| `0x80` |

In order to test the input, you can then check the register value to check each pad key is pressed.

*Example*
``` C
char joypad_register = get_current_gamepad();
if(joypad_register & (JOYPAD_A | JOYPAD_B))
{
    // Do something if both A and B are pressed...
}
```

!!! warning

    If you check if a button has been pressed at each frame, you are going to enter your if block very quickly, you may want to save the previous frame state of the gamepad (for example if you just want to pause the game)

[TODO] : La lib prendra certainement en charge ce genre de check
[TODO] : Attendre la définition de la lib pour parler des constantes (si on les expose) de l'état du joypad
