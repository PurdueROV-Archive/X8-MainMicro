//CHANGES FOR FIRDAY:
/*
 * The values passed to the thrustmapping (representing the thrust vector sent from
 * the battle station need to be scaled for use in the thrustmapping code.
 *
 * First all of the X, Y, and Z values must be multiplied by a value, for now we use
 * --------- 1024 ---------
 *
 * Then the X value must be downscaled by 
 * --------- 276  ---------
 *
 * Then the Y value must be downscaled by
 * --------- 765  ---------
 *
 * Then the Z value must be downscaled by
 * --------- 280  ---------
 *
 * There may be other changes necessary
 *
 * This will code will fail if a motor goes offline, in which case the scaling will
 * be done directly in the battle station
 *
 * The values are subject to change if the thrustmapping math is changed, so use the
 * defines here
 *
 * I don't know where everything should be modified, so Ryan told me to add the defines
 * so you can put everything where it belongs on friday.
 * -Jason
 */


#define PREMULTIPLY 1024
#define X_DOWNSCALE 276
#define Y_DOWNSCALE 765
#define Z_DOWNSCALE 280

#include "packet_in.h"


PacketIn::PacketIn() {}


uint8_t PacketIn::checksum(uint8_t *bytes) {
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


void PacketIn::recieve() {

    if (PacketIn::checksum(recieveBuffer) == recieveBuffer[PACKET_IN_LENGTH - 2]) {
        LedOn(RED);


        //Copy 6 int16_t thruster values
        memcpy(&thrusters[0], &recieveBuffer[2], 12);


        //Extract 
        cameraServo    = recieveBuffer[14];
        solenoids      = recieveBuffer[15];
        hydraulicsPump = recieveBuffer[16];
        leds           = recieveBuffer[17];
        thruster       = recieveBuffer[18];
        PIDControl     = recieveBuffer[19];


        //Copy 3 int16_t PID Tuning Values
        memcpy(&PIDTuning[0], &recieveBuffer[20], 6);

        //Copy 3 int8_t PID Pivot Values
        memcpy(&PIDPivot[0], &recieveBuffer[26], 3);
    } else {
       LedOff(RED);
    }
}

uint8_t* PacketIn::getArray() { 
    return recieveBuffer;
}

int16_t* PacketIn::getThrusters() {
    return thrusters;
}

uint8_t PacketIn::getCameraServo() { 
    return cameraServo;
}

uint8_t PacketIn::getSolenoids() { 
    return solenoids;
}

uint8_t PacketIn::getHydraulicsPump() { 
    return hydraulicsPump; 
}

uint8_t PacketIn::getLeds() {
    return leds;
}

uint8_t PacketIn::getPIDControl() {
    return PIDControl;
}

int16_t* PacketIn::getPIDTuning() {
    return (int16_t *) PIDTuning; 
}

int8_t* PacketIn::getPIDPivot() { 
    return PIDPivot;
}
