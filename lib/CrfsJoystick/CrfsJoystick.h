/*
  Joystick.h

  Copyright (c) 2015-2017, Matthew Heironimus

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef JOYSTICK_h
#define JOYSTICK_h

#include "DynamicHID.h"
#include "CrfsJoystickDefinition.h"

#if ARDUINO < 10606
#error The Joystick library requires Arduino IDE 1.6.6 or greater. Please update your IDE.
#endif // ARDUINO < 10606

#if ARDUINO > 10606
#if !defined(USBCON)
#error The Joystick library can only be used with a USB MCU (e.g. Arduino Leonardo, Arduino Micro, etc.).
#endif // !defined(USBCON)
#endif // ARDUINO > 10606

class CrfsJoystick
{
private:

    // Joystick State
    int32_t   _axisValues[12]; // A, E, T, R, slider 1-8
    HidReportInput1 _hidReportInput1;

protected:
    int buildAndSet16BitValue(bool includeValue, int32_t value, int32_t valueMinimum, int32_t valueMaximum, int32_t actualMinimum, int32_t actualMaximum, uint8_t dataLocation[]);
    int buildAndSetAxisValue(bool includeAxis, int32_t axisValue, int32_t axisMinimum, int32_t axisMaximum, uint8_t dataLocation[]);
    int buildAndSetSimulationValue(bool includeValue, int32_t value, int32_t valueMinimum, int32_t valueMaximum, uint8_t dataLocation[]);

public:
    CrfsJoystick();

    void begin(bool initAutoSendState = true);
    void end();

    // Set Axis Values
    void setAxis(size_t axis, int32_t value, int32_t valueMinimum, int32_t valueMaximum);
    void setButton(uint8_t button, bool value);

    void sendState();
};

#endif // JOYSTICK_h
