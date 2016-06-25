/* Packet index and corresponding values
 *
 * 00 	Header      0x12 - 18 decimal
 * 01   Packet Type 0x01 - Sensor Data
 * 02 	Ths. Status 1 bit per thruster
 * 03   Pressure-1  32-bit float
 * 04   Pressure-2  32-bit float
 * 05   Pressure-3  32-bit float
 * 06   Pressure-4  32-bit float
 * 07   Temp-1      32-bit float
 * 08   Temp-2      32-bit float
 * 09   Temp-3      32-bit float
 * 10   Temp-4      32-bit float
 * 11   IMU Lx-1    Signed int (16 bit)
 * 12   IMU Lx-2    Signed int (16 bit)
 * 13   IMU Ly-1    Signed int (16 bit)
 * 14   IMU Ly-2    Signed int (16 bit)
 * 15   IMU Lz-1    Signed int (16 bit)
 * 16   IMU Lz-2    Signed int (16 bit)
 * 17   IMU Rx-1    Signed int (16 bit)
 * 18   IMU Rx-2    Signed int (16 bit)
 * 19   IMU Ry-1    Signed int (16 bit)
 * 20   IMU Ry-2    Signed int (16 bit)
 * 21   IMU Rz-1    Signed int (16 bit)
 * 22   IMU Rz-2    Signed int (16 bit)
 * 23   CRC8 Check  Use 0xD5 as polynomial
 * 24   Tail Byte   0x13 - 19 decimal
 */


#include "packet_out.h"
#include "main.h"

//use constructor and add header and tail byte
PacketOut::PacketOut() {
    packetData[0]                   = 0x12;
    packetData[1]                   = 0x01;
    packetData[PACKET_OUT_LENGTH-1] = 0x13;
}

//make checksum function
char PacketOut::checksum(char* bytes) {
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

void PacketOut::setThrusterStatus(uint16_t data){
    memcpy(&packetData[2], &data, 1);
}

void PacketOut::setPressure(float data){
    memcpy(&packetData[3], &data, 4);
}

void PacketOut::setTemp(float data){
    memcpy(&packetData[7], &data, 4);
}

void PacketOut::setIMU_Lx(int16_t data){
    memcpy(&packetData[11], &data, 2);
}

void PacketOut::setIMU_Ly(int16_t data){
    memcpy(&packetData[13], &data, 2);
}

void PacketOut::setIMU_Lz(int16_t data){
    memcpy(&packetData[15], &data, 2);
}

void PacketOut::setIMU_Rx(int16_t data){
    memcpy(&packetData[17], &data, 2);
}

void PacketOut::setIMU_Ry(int16_t data){
    memcpy(&packetData[19], &data, 2);
}

void PacketOut::setIMU_Rz(int16_t data){
    memcpy(&packetData[21], &data, 2);
}

void PacketOut::setThrusters(int8_t* data) {
    memcpy(&packetData[23], data, 8);
}

void PacketOut::send() {
    //Compute checksum
    packetData[PACKET_OUT_LENGTH-2] = checksum((char*) packetData);

    //Send
    HAL_UART_Transmit(&huart3, packetData, PACKET_OUT_LENGTH, 100);
}
