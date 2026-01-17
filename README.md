# CRSFJoystickProMicro
A clone of [robhaswell/CRSFJoystickProMicro](https://github.com/robhaswell/CRSFJoystickProMicro).

This fork uses a custom USB HID descriptor, which mocks the one one my RadioMaster TX15. 

Uses 8 Axis in order X,Y,Z,Rx,Ry,Rz,Slider,Dial for CRFS Channels 1-8. 
Uses 24 Buttons, from which the first 8 are mapped to CRGS Channels 5-12.

***The Baud Rate of the receiver has to be 250000. This can be configured through WiFi.***

## BOM
- Arduino SS Micro form Ebay.
- BetaFPV ELRS Lite Receiver (Flat Antenna).
- Cables
- Heatschrink tubing

## Build
![Components](https://github.com/LeonRein/CRSFJoystickProMicro/blob/master/img/1.jpg)
![Wiring](https://github.com/LeonRein/CRSFJoystickProMicro/blob/master/img/2.jpg)
![Finished](https://github.com/LeonRein/CRSFJoystickProMicro/blob/master/img/3.jpg)
![Running](https://github.com/LeonRein/CRSFJoystickProMicro/blob/master/img/4.jpg)
