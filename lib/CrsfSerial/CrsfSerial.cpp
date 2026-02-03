#include "CrsfSerial.h"

CrsfSerial::CrsfSerial(HardwareSerial &port, uint32_t baud, uint16_t delay_ms) :
    _port(port), _crc(0xd5), _baud(baud), _delay_ms(delay_ms),
    _lastReceive(0), _lastChannelsPacket(0), _linkIsUp(false),
    _passthroughMode(false)
{
    // Crsf serial is 420000 baud for V2
    _port.begin(_baud);
}

// Call from main loop to update
void CrsfSerial::loop()
{
    handleSerialIn();
    if (!_passthroughMode)
        handleDelayedChannels();
}

void CrsfSerial::handleSerialIn()
{
    while (_port.available())
    {
        uint8_t b = _port.read();
        _lastReceive = millis();

        if (_passthroughMode)
        {
            if (onShiftyByte)
                onShiftyByte(b);
            continue;
        }

        _rxBuf[_rxBufPos++] = b;
        handleByteReceived();

        if (_rxBufPos == (sizeof(_rxBuf)/sizeof(_rxBuf[0])))
        {
            // Packet buffer filled and no valid packet found, dump the whole thing
            _rxBufPos = 0;
        }
    }

    checkPacketTimeout();
    checkLinkDown();
}

void CrsfSerial::handleByteReceived()
{
    bool reprocess;
    do
    {
        reprocess = false;
        if (_rxBufPos > 1)
        {
            uint8_t len = _rxBuf[1];
            // Sanity check the declared length, can't be shorter than Type, X, CRC
            if (len < 3 || len > CRSF_MAX_PACKET_LEN)
            {
                shiftRxBuffer(1);
                reprocess = true;
            }

            else if (_rxBufPos >= (len + 2))
            {
                uint8_t inCrc = _rxBuf[2 + len - 1];
                uint8_t crc = _crc.calc(&_rxBuf[2], len - 1);
                if (crc == inCrc)
                {
                    processPacketIn(len);
                    shiftRxBuffer(len + 2);
                    reprocess = true;
                }
                else
                {
                    shiftRxBuffer(1);
                    reprocess = true;
                }
            }  // if complete packet
        } // if pos > 1
    } while (reprocess);
}

void CrsfSerial::checkPacketTimeout()
{
    // If we haven't received data in a long time, flush the buffer a byte at a time (to trigger shiftyByte)
    if (_rxBufPos > 0 && millis() - _lastReceive > CRSF_PACKET_TIMEOUT_MS)
        while (_rxBufPos)
            shiftRxBuffer(1);
}

void CrsfSerial::checkLinkDown()
{
    if (_linkIsUp && millis() - _lastChannelsPacket > CRSF_FAILSAFE_STAGE1_MS)
    {
        if (onLinkDown)
            onLinkDown();
        _linkIsUp = false;
    }
}

void CrsfSerial::processPacketIn(uint8_t len)
{
    const crsf_header_t *hdr = (crsf_header_t *)_rxBuf;
    if (hdr->device_addr == CRSF_ADDRESS_FLIGHT_CONTROLLER)
    {
        switch (hdr->type)
        {
        case CRSF_FRAMETYPE_GPS:
            packetGps(hdr);
            break;
        case CRSF_FRAMETYPE_RC_CHANNELS_PACKED:
            packetChannelsPacked(hdr);
            break;
        case CRSF_FRAMETYPE_LINK_STATISTICS:
            packetLinkStatistics(hdr);
            break;
        }
    } // CRSF_ADDRESS_FLIGHT_CONTROLLER
}

// Shift the bytes in the RxBuf down by cnt bytes
void CrsfSerial::shiftRxBuffer(uint8_t cnt)
{
    // If removing the whole thing, just set pos to 0
    if (cnt >= _rxBufPos)
    {
        _rxBufPos = 0;
        return;
    }

    if (cnt == 1 && onShiftyByte)
        onShiftyByte(_rxBuf[0]);

    // Otherwise do the slow shift down
    uint8_t *src = &_rxBuf[cnt];
    uint8_t *dst = &_rxBuf[0];
    _rxBufPos -= cnt;
    uint8_t left = _rxBufPos;
    while (left--)
        *dst++ = *src++;
}

void CrsfSerial::packetChannelsPacked(const crsf_header_t *p)
{
    crsf_channels_t *ch = (crsf_channels_t *)&p->data;
    timestamp_channels_s *tsch = _txBuffer.getWriteSlot();
    if (tsch == nullptr) {
        SerialUSB.println("Buffer full, dropping channels packet");
        return; // Buffer full, drop packet
    }

    tsch->timestamp = millis();

    auto rawChannels = tsch->channels;

    rawChannels[0] = ch->ch0;
    rawChannels[1] = ch->ch1;
    rawChannels[2] = ch->ch2;
    rawChannels[3] = ch->ch3;
    rawChannels[4] = ch->ch4;
    rawChannels[5] = ch->ch5;
    rawChannels[6] = ch->ch6;
    rawChannels[7] = ch->ch7;
    rawChannels[8] = ch->ch8;
    rawChannels[9] = ch->ch9;
    rawChannels[10] = ch->ch10;
    rawChannels[11] = ch->ch11;
    rawChannels[12] = ch->ch12;
    rawChannels[13] = ch->ch13;
    rawChannels[14] = ch->ch14;
    rawChannels[15] = ch->ch15;

    
    for (unsigned int i=0; i<CRSF_NUM_CHANNELS; ++i)
        rawChannels[i] = map(rawChannels[i], CRSF_CHANNEL_VALUE_1000, CRSF_CHANNEL_VALUE_2000, 1000, 2000);
    
    _txBuffer.commit();

    if (!_linkIsUp && onLinkUp)
        onLinkUp();
    _linkIsUp = true;
    _lastChannelsPacket = millis();
}

void CrsfSerial::handleDelayedChannels()
{
    if (!_txBuffer.isEmpty() && _txBuffer.peek().timestamp + _delay_ms <= millis())
    {
        timestamp_channels_s tsch;
        // Pop the item
        _txBuffer.pop(tsch);

        // Copy the channels
        memcpy(_channels, tsch.channels, sizeof(_channels));

        static unsigned int cnt = 0;
        cnt++;

        if (cnt % 250 == 0)
        {
            Serial.print("Delay:");
            Serial.print(millis() - tsch.timestamp);
            Serial.print(", buffer size: ");
            Serial.println(_txBuffer.size());
        }

        if (onPacketChannels)
            onPacketChannels();
    }
}

void CrsfSerial::packetLinkStatistics(const crsf_header_t *p)
{
    const crsfLinkStatistics_t *link = (crsfLinkStatistics_t *)p->data;
    memcpy(&_linkStatistics, link, sizeof(_linkStatistics));

    if (onPacketLinkStatistics)
        onPacketLinkStatistics(&_linkStatistics);
}

void CrsfSerial::packetGps(const crsf_header_t *p)
{
    const crsf_sensor_gps_t *gps = (crsf_sensor_gps_t *)p->data;
    _gpsSensor.latitude = be32toh(gps->latitude);
    _gpsSensor.longitude = be32toh(gps->longitude);
    _gpsSensor.groundspeed = be16toh(gps->groundspeed);
    _gpsSensor.heading = be16toh(gps->heading);
    _gpsSensor.altitude = be16toh(gps->altitude);
    _gpsSensor.satellites = gps->satellites;

    if (onPacketGps)
        onPacketGps(&_gpsSensor);
}

void CrsfSerial::write(uint8_t b)
{
    _port.write(b);
}

void CrsfSerial::write(const uint8_t *buf, size_t len)
{
    _port.write(buf, len);
}

void CrsfSerial::queuePacket(uint8_t addr, uint8_t type, const void *payload, uint8_t len)
{
    if (!_linkIsUp)
        return;
    if (_passthroughMode)
        return;
    if (len > CRSF_MAX_PACKET_LEN)
        return;

    uint8_t buf[CRSF_MAX_PACKET_LEN+4];
    buf[0] = addr;
    buf[1] = len + 2; // type + payload + crc
    buf[2] = type;
    memcpy(&buf[3], payload, len);
    buf[len+3] = _crc.calc(&buf[2], len + 1);

    // Busywait until the serial port seems free
    //while (millis() - _lastReceive < 2)
    //    loop();
    write(buf, len + 4);
}

void CrsfSerial::setPassthroughMode(bool val, unsigned int baud)
{
    _passthroughMode = val;
    _port.flush();
    if (baud != 0)
        _port.begin(baud);
    else
        _port.begin(_baud);
}
