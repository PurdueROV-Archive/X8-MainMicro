//
// Created by stewa on 2/2/2016.
//

#ifndef X8_MAINMICRO_PACKET_H
#define X8_MAINMICRO_PACKET_H


//Buffer Size for Ethernet
#include "main.h"


#define SERIAL_BUFFER_SIZE 30


class PacketIn {
public :
    void recieve();

    uint8_t * getArray();
    PacketIn();
    int16_t * getThrusters();

private :

    char recieveBuffer[SERIAL_BUFFER_SIZE];


    char checksum(char *bytes);



    //char header; //for reference

    //Fields to fill in
    int16_t thrusters[6];
    uint8_t solenoids;
    uint8_t hydaulicsPump;
    uint8_t Leds;
    uint8_t thruster;
    uint8_t PIDControl;
    int16_t PIDTuning[3];
    int8_t PIDPivot[3];


    //char checksum; //for reference
    //char tail; //for reference

};

#endif //X8_MAINMICRO_PACKET_H

