//
// Created by stewa on 2/6/2016.
//

#ifndef X8_MAINMICRO_PACKETOUT_H
#define X8_MAINMICRO_PACKETOUT_H

#include "main.h"
#include "string.h"

#define PACKET_LENGTH 36

class PacketOut {
public:
    void send();
    void setPressure(float data);
    void setThrusterStatus(uint32_t data);
    void setTemp(uint32_t data);

	void setIMU_Lx(float data){
	void setIMU_Ly(float data){
	void setIMU_Lz(float data){
	void setIMU_Rx(float data){
	void setIMU_Ry(float data){
	void setIMU_Rz(float data){


private:
    char sendBuffer[SERIAL_OUT_BUFFER_SIZE];
    uint8_t Dataup[SERIAL_OUT_BUFFER_SIZE];

    char checksum(char *bytes);
};




#endif //X8_MAINMICRO_PACKETOUT_H
