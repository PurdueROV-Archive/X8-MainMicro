/* Packet index and corresponding values
 *
 * 00 	Header      0x12 - 18 decimal
 * 01 	Ths. Status 1 bit per thruster
 * 02  Pressure-1  32-bit float
 * 03  Pressure-2  32-bit float
 * 04  Pressure-3  32-bit float
 * 05  Pressure-4  32-bit float
 * 06  Temp-1      32-bit float
 * 07  Temp-2      32-bit float
 * 08  Temp-3      32-bit float
 * 09  Temp-4      32-bit float
 * 10  IMU Lx-1    32-bit float
 * 11  IMU Lx-2    32-bit float
 * 12  IMU Lx-3    32-bit float
 * 13  IMU Lx-4    32-bit float
 * 14  IMU Ly-1    32-bit float
 * 15  IMU Ly-2    32-bit float
 * 16  IMU Ly-3    32-bit float
 * 17  IMU Ly-4    32-bit float
 * 18  IMU Lz-1    32-bit float
 * 19  IMU Lz-2    32-bit float
 * 20  IMU Lz-3    32-bit float
 * 21  IMU Lz-4    32-bit float
 * 22  IMU Rx-1    32-bit float
 * 23  IMU Rx-2    32-bit float
 * 24  IMU Rx-3    32-bit float
 * 25  IMU Rx-4    32-bit float
 * 26  IMU Ry-1    32-bit float
 * 27  IMU Ry-2    32-bit float
 * 28  IMU Ry-3    32-bit float
 * 29  IMU Ry-4    32-bit float
 * 30  IMU Rz-1    32-bit float
 * 31  IMU Rz-2    32-bit float
 * 32  IMU Rz-3    32-bit float
 * 33  IMU Rz-4    32-bit float
 * 34  CRC8 Check  Use 0xD5 as polynomial
 * 35  Tail Byte   0x13 - 19 decimal
 */


#include "packet_out.h"
#include "main.h"

//use constructor and add header and tail byte
PacketOut::PacketOut() {
    Dataup[0]=0x12;
    Dataup[PACKET_OUT_LENGTH-1]=0x13;
}

//make checksum function
char PacketOut::checksum(char *bytes) {
    char crc = 0;
    char val;
    char mix;
    for (int i = 1; i < PACKET_OUT_LENGTH - 2; ++i) {
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

void PacketOut::setThrusterStatus(uint32_t data){
    memcpy(&Dataup[1], &data, 1);
}

void PacketOut::setPressure(float data){
    memcpy(&Dataup[2], &data, 4);
}

void PacketOut::setTemp(uint32_t data){
    memcpy(&Dataup[6], &data, 4);
}

void PacketOut::setIMU_Lx(float data){
    memcpy(&Dataup[10], &data, 4);
}

void PacketOut::setIMU_Ly(float data){
    memcpy(&Dataup[14], &data, 4);
}

void PacketOut::setIMU_Lz(float data){
    memcpy(&Dataup[18], &data, 4);
}

void PacketOut::setIMU_Rx(float data){
    memcpy(&Dataup[22], &data, 4);
}

void PacketOut::setIMU_Ry(float data){
    memcpy(&Dataup[26], &data, 4);
}

void PacketOut::setIMU_Rz(float data){
    memcpy(&Dataup[30], &data, 4);
}

void PacketOut::send() {

    Dataup[PACKET_OUT_LENGTH-2] = checksum((char*)Dataup);

    HAL_UART_Transmit_DMA(&huart3, Dataup, PACKET_OUT_LENGTH);
}
