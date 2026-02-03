#include <CrsfSerial.h>  // https://github.com/CapnBry/CRServoF/
#include <CrfsJoystick.h>    // https://github.com/MHeironimus/ArduinoJoystickLibrary
#include <board.h>

#include "calibration.h"

CrsfSerial crsf(Serial1, AVR_CRSF_BAUDRATE, 15);

int channel_data = 0;
int map_data = 0;

CrfsJoystick joystick;

/***
 * This callback is called whenever new channel values are available.
 * Use crsf.getChannel(x) to get us channel values (1-16).
 ***/
void packetChannels() {
  for (uint8_t i = 0; i < NUM_AXIS; i++) {
    axis_config *cfg = &axis_map[i];
    channel_data = crsf.getChannel(cfg->channel);
    joystick.setAxis(cfg->id, channel_data, cfg->low_ep, cfg->high_ep);
  }

  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    btn_config *cfg = &btn_map[i];
    channel_data = crsf.getChannel(cfg->channel);
    // bounds check inclusive
    if (channel_data >= cfg->lower_bound && channel_data <= cfg->upper_bound) {
      map_data = cfg->invert ? LOW : HIGH;
    } else {
      map_data = cfg->invert ? HIGH : LOW;
    }
    joystick.setButton(cfg->id, map_data);
  }
  joystick.sendState();
}

void crsfLinkUp() {
  ledOn();
}

void crsfLinkDown() {
  ledOff();
}

void setup() {
  delay(1000);  // Wait for USB Serial to initialize
  Serial.begin(115200);
  ledOff();

  joystick.begin();  // Do not send updates automatically

  // Attach the channels callback
  crsf.onPacketChannels = &packetChannels;
  crsf.onLinkUp = &crsfLinkUp;
  crsf.onLinkDown = &crsfLinkDown;
}

void loop() {
  // Must call CrsfSerial.loop() in loop() to process data
  crsf.loop();
}