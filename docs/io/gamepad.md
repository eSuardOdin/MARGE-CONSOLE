# Gamepad

R0se's gamepad is a simple old school game pad, it contains only 8 buttons.

It fits into a 8 bit register and each bit can be tested against a mask to know wich keys are being pressed.

The bit corresponding to a button is pressed if it's value is `1`, if it is `0`, the button is released.

|  Key    | Constant name | Flag  |
|---------|---------------|-------|
| Up key  | `JOYPAD_UP`   | `0x1` |
| Up key  | `JOYPAD_UP`   | `0x1` |
| Up key  | `JOYPAD_UP`   | `0x1` |
| Up key  | `JOYPAD_UP`   | `0x1` |
| Up key  | `JOYPAD_UP`   | `0x1` |