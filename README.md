# CRSFJoystickProMicro
A clone of [robhaswell/CRSFJoystickProMicro](https://github.com/robhaswell/CRSFJoystickProMicro).

This fork uses a custom USB HID descriptor, which mocks the one one my RadioMaster TX15. 

Uses 8 Axis in order X,Y,Z,Rx,Ry,Rz,Slider,Dial for CRFS Channels 1-8. 
Uses 24 Buttons, from which the first 8 are mapped to CRGS Channels 5-12.

***If you build this project you MUST flash your receiver with `RCVR_UART_BAUD=250000`.***

