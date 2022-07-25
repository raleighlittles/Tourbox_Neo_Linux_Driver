# Button Mapping

Button mapping names come from: https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h

| Button name                       | 'Scan Code'  (Hex) | Key mapped to                 |
|-----------------------------------|--------------------|-------------------------------|
| SIDE_BUTTON                       | 81                 | KEY_MUTE                      |
| SCROLL_WHEEL_UP                   | [49][c9]           | KEY_SCROLLUP                  |
| SCROLL_WHEEL_DOWN                 | [09][89]           | KEY_SCROLLDOWN                |
| SCROLL_WHEEL_CLICK                | [0a][8a]           | KEY_PAUSE                     |
| BUTTON_3                          | 82                 | KEY_ENTER                     |
| BUTTON_A                          | [22][a2]           | BTN_LEFT (Mouse left click)   |
| BUTTON_B                          | [23][a3]           | BTN_RIGHT (Mouse right click) |
| BUTTON_6                          | 83                 | KEY_DOT                       |
| BUTTON_7                          | 80                 | KEY_MENU                      |
| BUTTON_DPAD_UP                    | 90                 | Mouse up                      |
| BUTTON_DPAD_RIGHT                 | 93                 | Mouse right                   |
| BUTTON_DPAD_DOWN                  | 91                 | Mouse down                    |
| BUTTON_DPAD_LEFT                  | 92                 | Mouse left                    |
| IPOD_WHEEL_CLOCKWISE              | 8f                 | KEY_VOLUMEUP                  |
| IPOD_WHEEL_COUNTERCLOCKWISE       | [4f][cf]           | KEY_VOLUMEDOWN                |
| BIG_CENTER_WHEEL_CLOCKWISE        | [44][c4]           | KEY_BRIGHTNESSUP              |
| BIG_CENTER_WHEEL_COUNTERCLOCKWISE | 84                 | KEY_BRIGHTNESSDOWN            |
| BUTTON_11                         | [2a][aa]           |                               |

![annotated version](./tourbox-stock-image-annotated.jpg)


# Serial port settings

(From Picocom)

```
*** baud: 112500 (112676)
*** flow: none
*** parity: none
*** databits: 8
*** stopbits: 1
*** dtr: up
*** rts: up
*** mctl: DTR:1 DSR:0 DCD:0 RTS:1 CTS:0 RI:0
```