//
// Created by stewa on 2/6/2016.
//

#ifndef X8_MAINMICRO_PACKETOUT_H
#define X8_MAINMICRO_PACKETOUT_H

#include "main.h"

#define SERIAL_OUT_BUFFER_SIZE  28

#define ThrusterStatus 1
#define Pressure 2
#define Temp 3
#define IMUA 8
#define IMUB 10
#define IMUC 12
#define IMUD 14
#define IMUE 16
#define IMUF 18
#define IMUG 20
#define IMUH 22
#define IMUI 24
#define check 26


class PacketOut{
public:
    void send();

    void setData(int,uint32_t);
    PacketOut(UART_HandleTypeDef* serialStruct);




private:
    char sendBuffer[SERIAL_OUT_BUFFER_SIZE];
    uint8_t Dataup[SERIAL_OUT_BUFFER_SIZE];

    char tempBuffer[SERIAL_OUT_BUFFER_SIZE-2];


    char checksum(char *bytes);
};




#endif //X8_MAINMICRO_PACKETOUT_H
