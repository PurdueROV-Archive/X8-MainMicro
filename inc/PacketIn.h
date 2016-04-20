#ifndef X8_MAINMICRO_PACKET_H
#define X8_MAINMICRO_PACKET_H


//Buffer Size for Ethernet
#include "main.h"


#define PACKET_IN_LENGTH	31
#define HEADER              0x12
#define TAIL                0x13


class PacketIn {
public:
    PacketIn();

    void     recieve();
    uint8_t* getArray();
    int16_t* getThrusters();
    uint8_t  getCameraServo();
    uint8_t  getSolenoids();
    uint8_t  getHydraulicsPump();
    uint8_t  getLeds();
    uint8_t  getThruster();
    uint8_t  getPIDControl();
    int16_t* getPIDTuning();
    int8_t*  getPIDPivot();

private:

    uint8_t recieveBuffer[PACKET_IN_LENGTH];

    uint8_t checksum(uint8_t *bytes);

    //Fields to fill in
    int16_t thrusters[6];
    uint8_t cameraServo;
    uint8_t solenoids;
    uint8_t hydraulicsPump;
    uint8_t leds;
    uint8_t thruster;
    uint8_t PIDControl;
    int16_t PIDTuning[3];
    int8_t  PIDPivot[3];
};

#endif //X8_MAINMICRO_PACKET_H
