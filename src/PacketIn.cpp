//

// Created by Brandon 2/2/2016.

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


#include "PacketIn.h"
#include "string.h"


PacketIn::PacketIn() {

}

char PacketIn::checksum(char *bytes) {
    char crc = 0;
    char val;
    char mix;
    for (int i = 1; i < SERIAL_IN_BUFFER_SIZE - 2; ++i) {
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



    if (PacketIn::checksum(recieveBuffer) == recieveBuffer[SERIAL_IN_BUFFER_SIZE - 2]) {


            memcpy(&thrusters[0], &recieveBuffer[2], 2);
            memcpy(&thrusters[1], &recieveBuffer[4], 2);
            memcpy(&thrusters[2], &recieveBuffer[6], 2);


            memcpy(&thrusters[3], &recieveBuffer[8], 2);
            memcpy(&thrusters[4], &recieveBuffer[10], 2);
            memcpy(&thrusters[5], &recieveBuffer[12], 2);

        solenoids = (uint8_t) recieveBuffer[14];
        hydraulicsPump = (uint8_t) recieveBuffer[15];
        leds = (uint8_t) recieveBuffer[16];
        thruster = (uint8_t) recieveBuffer[17];
        PIDControl = (uint8_t) recieveBuffer[18];


        PIDTuning[0] = recieveBuffer[19];
        PIDTuning[0] =PIDTuning[0] << 8;
        PIDTuning[0] += recieveBuffer[20];

        PIDTuning[1] = recieveBuffer[21];
        PIDTuning[1] = PIDTuning[1] << 8;
        PIDTuning[1] += recieveBuffer[22];

        PIDTuning[2] = recieveBuffer[23];
        PIDTuning[2] = PIDTuning[2] << 8;
        PIDTuning[2] += recieveBuffer[24];

        PIDPivot[0] = recieveBuffer[25];
        PIDPivot[1] = recieveBuffer[26];
        PIDPivot[2] = recieveBuffer[27];


    }
    else {

    }
}




uint8_t * PacketIn::getArray() { return (uint8_t *) recieveBuffer; }

int16_t * PacketIn::getThrusters() { return thrusters; }

uint8_t PacketIn::getSolenoids() { return (uint8_t) solenoids; }
uint8_t PacketIn::getHydraulicsPump() { return (uint8_t)  hydraulicsPump; }
uint8_t PacketIn::getLeds() { return (uint8_t)  leds; }
uint8_t PacketIn::getPIDControl() { return (uint8_t)  PIDControl; }
int16_t * PacketIn::getPIDTuning() { return (int16_t *) PIDTuning; }
int8_t * PacketIn::getPIDPivot() { return (int8_t *) PIDPivot; }


