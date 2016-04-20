#ifndef X8_MAINMICRO_PACKETOUT_H
#define X8_MAINMICRO_PACKETOUT_H

#include "main.h"

#define PACKET_OUT_LENGTH 36

class PacketOut {
public:
    PacketOut();

    void send();
    void setPressure(float data);
    void setThrusterStatus(uint32_t data);
    void setTemp(uint32_t data);

    void setIMU_Lx(float data);
    void setIMU_Ly(float data);
    void setIMU_Lz(float data);
    void setIMU_Rx(float data);
    void setIMU_Ry(float data);
    void setIMU_Rz(float data);


private:
    char sendBuffer[PACKET_OUT_LENGTH];
    uint8_t Dataup[PACKET_OUT_LENGTH];

    char checksum(char *bytes);
};

#endif //X8_MAINMICRO_PACKETOUT_H
