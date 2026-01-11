/*
  Joystick.cpp

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

#include "CrfsJoystick.h"

#define JOYSTICK_AXIS_MINIMUM (-32768)
#define JOYSTICK_AXIS_MAXIMUM 32767

CrfsJoystick::CrfsJoystick()
{	
	// Register HID Report Description
	DynamicHIDSubDescriptor *node = new DynamicHIDSubDescriptor(hidReportDescriptor, sizeof(hidReportDescriptor), false);
	DynamicHID().AppendDescriptor(node);

	_hidReportInput1 = HidReportInput1();
}

void CrfsJoystick::begin(bool initAutoSendState)
{
	sendState();
}

void CrfsJoystick::end()
{
}

void CrfsJoystick::setAxis(size_t axis, int32_t value, int32_t valueMinimum, int32_t valueMaximum)
{
  int32_t constrainedValue = constrain(value, valueMinimum, valueMaximum);
	int32_t convertedValue = map(constrainedValue, valueMinimum, valueMaximum, JOYSTICK_AXIS_MINIMUM, JOYSTICK_AXIS_MAXIMUM);

	uint8_t highByte = (uint8_t)(convertedValue >> 8);
	uint8_t lowByte = (uint8_t)(convertedValue & 0x00FF);
	
	_hidReportInput1.Payload[3 + axis * 2] = lowByte;
	_hidReportInput1.Payload[3 + axis * 2 + 1] = highByte;
}

void CrfsJoystick::setButton(uint8_t button, bool value)
{
  if (button > 23) {
    return; // Out of range
  }

  uint8_t byteIndex = button / 8;
  uint8_t bitIndex = button % 8;

  if (value) {
    _hidReportInput1.Payload[byteIndex] |= (1 << bitIndex);
  } else {
    _hidReportInput1.Payload[byteIndex] &= ~(1 << bitIndex);
  }
}

void CrfsJoystick::sendState()
{	
	DynamicHID().SendReport(_hidReportInput1.ReportId, _hidReportInput1.Payload, sizeof(_hidReportInput1.Payload));
}
