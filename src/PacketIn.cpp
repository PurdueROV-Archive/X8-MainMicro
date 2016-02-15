//
// Created by stewa on 2/2/2016.
//

#include "PacketIn.h"


    PacketIn::PacketIn() {

    }

    char PacketIn::checksum(char *bytes) {
        char crc = 0;
        char val;
        char mix;
        for (int i = 1; i < SERIAL_BUFFER_SIZE - 2; ++i) {
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

        //for (int i = 0; i < SERIAL_BUFFER_SIZE; i++) {
        this->recieveBuffer[2] = (uint8_t) 'S';
        //}
        if (PacketIn::checksum(recieveBuffer)) {

            thrusters[0] = recieveBuffer[2];
            thrusters[0] = thrusters[0] << 8;
            thrusters[0] += recieveBuffer[3];

            thrusters[1] = recieveBuffer[4];
            thrusters[1] = thrusters[1] << 8;
            thrusters[1] += recieveBuffer[5];

            thrusters[2] = recieveBuffer[6];
            thrusters[2] = thrusters[2] << 8;
            thrusters[2] += recieveBuffer[7];

            thrusters[3] = recieveBuffer[8];
            thrusters[3] = thrusters[3] << 8;
            thrusters[3] += recieveBuffer[9];

            thrusters[4] = recieveBuffer[10];
            thrusters[4] = thrusters[4] << 8;
            thrusters[4] += recieveBuffer[11];

            thrusters[5] = recieveBuffer[12];
            thrusters[5] = thrusters[5] << 8;
            thrusters[5] += recieveBuffer[13];


            solenoids = (uint8_t) recieveBuffer[14];
            hydaulicsPump = (uint8_t) recieveBuffer[15];
            Leds = (uint8_t) recieveBuffer[16];
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

    uint8_t * PacketIn::getArray() {
        //recieveBuffer[1] = (uint8_t) 'h';
        return (uint8_t *) recieveBuffer;
    }

    int16_t * PacketIn::getThrusters() {
        return thrusters;
    }