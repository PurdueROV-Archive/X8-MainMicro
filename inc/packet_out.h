#ifndef X8_MAINMICRO_PACKETOUT_H
#define X8_MAINMICRO_PACKETOUT_H

#include "main.h"

#define PACKET_OUT_LENGTH 25

class PacketOut {
public:
    PacketOut();

    void send();
    void setThrusterStatus(uint16_t data);
    void setPressure(float data);
    void setTemp(float data);

    void setIMU_Lx(int16_t data);
    void setIMU_Ly(int16_t data);
    void setIMU_Lz(int16_t data);
    void setIMU_Rx(int16_t data);
    void setIMU_Ry(int16_t data);
    void setIMU_Rz(int16_t data);


private:
    uint8_t packetData[PACKET_OUT_LENGTH];

    char checksum(char *bytes);
};

#endif //X8_MAINMICRO_PACKETOUT_H
