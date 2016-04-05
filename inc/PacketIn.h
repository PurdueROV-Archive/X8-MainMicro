//
// Created by stewa on 2/2/2016.
//

#ifndef X8_MAINMICRO_PACKET_H
#define X8_MAINMICRO_PACKET_H


//Buffer Size for Ethernet
#include "main.h"


#define SERIAL_IN_BUFFER_SIZE  31


class PacketIn {
public :
    void recieve();

    uint8_t * getArray();
    PacketIn();
    int16_t * getThrusters();
    uint8_t getSolenoids();
    uint8_t getHydraulicsPump();
    uint8_t getLeds();
    uint8_t getThruster();
    uint8_t getPIDControl();
    int16_t * getPIDTuning();
    int8_t * getPIDPivot();

private :

    char recieveBuffer[SERIAL_IN_BUFFER_SIZE];


    char checksum(char *bytes);



    //char header; //for reference

    //Fields to fill in
    int16_t thrusters[6];
    uint8_t solenoids;
    uint8_t hydraulicsPump;
    uint8_t leds;
    uint8_t thruster;
    uint8_t PIDControl;
    int16_t PIDTuning[3];
    int8_t PIDPivot[3];


    //char checksum; //for reference
    //char tail; //for reference

};

#endif //X8_MAINMICRO_PACKET_H

