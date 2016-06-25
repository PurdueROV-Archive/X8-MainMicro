#include "packet_in.h"

PacketIn::PacketIn() {}

uint8_t PacketIn::checksum(uint8_t* bytes) {
    uint8_t crc = 0;
    uint8_t val;
    uint8_t mix;

    for (int i = 1; i < PACKET_IN_LENGTH - 2; ++i) {
        val = bytes[i];
        for (int j = 8; j; --j) {
            mix = (crc ^ val) & 0x01;
            crc >>= 1;
            if (mix) {
                crc ^= 0xD5;
            }
            val >>= 1;
        }
    }
    return crc;
}

bool PacketIn::recieve() {

    if (PacketIn::checksum(recieveBuffer) == recieveBuffer[PACKET_IN_LENGTH - 2]) {
        LedOn(GREEN);

        //Copy 6 int16_t thruster values
        memcpy(&thrusters[0], &recieveBuffer[2], 12);

        //Extract 
        cameraServo = recieveBuffer[14];
        solenoids   = recieveBuffer[15];
        thruster    = recieveBuffer[16];
        PIDControl  = recieveBuffer[17];

        //Copy 4 int16_t PID Tuning Values
        memcpy(&PIDTuning[0], &recieveBuffer[18], 8);
        return true;
    } else {
        LedOff(GREEN);
        return false;
    }
}

uint8_t* PacketIn::getArray() { 
    return recieveBuffer;
}

int16_t* PacketIn::getThrusters() {
    return thrusters;
}

uint8_t PacketIn::getCameraServo() { 
    return (cameraServo - (-128)) * (90 - (-90)) / (127 - (-128)) + (-90);
}

uint8_t PacketIn::getSolenoids() { 
    return solenoids;
}

uint8_t PacketIn::getPIDControl() {
    return PIDControl;
}

int16_t* PacketIn::getPIDTuning() {
    return (int16_t*) PIDTuning; 
}

uint8_t PacketIn::getThruster() {
    return thruster;
}
