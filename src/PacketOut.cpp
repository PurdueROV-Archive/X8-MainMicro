// Ajay 02/20/16
//

/* Packet index and corresponding values
 *
 * *00 	Header 	0x12 - 18 decimal
 * *01 	Thruster Status 	1 bit per thruster
 * *02  Pressure-1  32bit-float (4 bytes), Highest order
 * *03  Pressure-2  32bit-float (4 bytes), 2nd-Highest order
 * *04  Pressure-3  32bit-float (4 bytes), 3rd-Highest order
 * *05  Pressure-4  32bit-float (4 bytes), Lowest order
 * *06  Temp-1      32-bit float, Highest order
 * *07  Temp-2      32-bit float, High order
 * *08  Temp-3      32-bit float, Low order
 * *09  Temp-4      32-bit float, Lowest order
 * *10  IMU A-1     32bit-float  (4 bytes), Highest order
 * *11  IMU A-2     32bit-float  (4 bytes), 2nd-Highest order
 * *12  IMU A-3     32bit-float  (4 bytes), 3rd-Highest order
 * *13  IMU A-4     32bit-float  (4 bytes), Lowest order
 * *14  IMU B-1     32bit-float  (4 bytes), Highest order
 * *15  IMU B-2     32bit-float  (4 bytes), 2nd-Highest order
 * *16  IMU B-3     32bit-float  (4 bytes), 3rd-Highest order
 * *17  IMU B-4     32bit-float  (4 bytes), Lowest order
 * *18  IMU C-1     32bit-float  (4 bytes), Highest order
 * *19  IMU C-2     32bit-float  (4 bytes), 2nd-Highest order
 * *20  IMU C-3     32bit-float  (4 bytes), 3rd-Highest order
 * *21  IMU C-4     32bit-float  (4 bytes), Lowest order
 * *22  IMU D-1     32bit-float  (4 bytes), Highest order
 * *23  IMU D-2     32bit-float  (4 bytes), 2nd-Highest order
 * *24  IMU D-3     32bit-float  (4 bytes), 3rd-Highest order
 * *25  IMU D-4     32bit-float  (4 bytes), Lowest order
 * *26  IMU E-1     32bit-float  (4 bytes), Highest order
 * *27  IMU E-2     32bit-float  (4 bytes), 2nd-Highest order
 * *28  IMU E-3     32bit-float  (4 bytes), 3rd-Highest order
 * *29  IMU E-4     32bit-float  (4 bytes), Lowest order
 * *30  IMU F-1     32bit-float  (4 bytes), Highest order
 * *31  IMU F-2     32bit-float  (4 bytes), 2nd-Highest order
 * *32  IMU F-3     32bit-float  (4 bytes), 3rd-Highest order
 * *33  IMU F-4     32bit-float  (4 bytes), Lowest order
 * *34  CRC8 Check  Use 0xD5 as polynomial
 * *35  Tail Byte   0x13 - 19 decimal
#define SERIAL_OUT_BUFFER_SIZE  28

//////////////////////////// # defines for reference //////////////////////////////////

#define ThrusterStatus 1 //////////////// # defines for reference
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

 */

#include "PacketOut.h"
#include "main.h"


PacketOut::PacketOut() //use constructor and add header and tail byte
{
    Dataup[0]=0x12;
    Dataup[27]=0x13;


}
//make checksum function
char PacketOut::checksum(char *bytes) {
    char crc = 0;
    char val;
    char mix;
    for (int i = 1; i < SERIAL_OUT_BUFFER_SIZE - 2; ++i) {
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
void PacketOut::setPressure(float data){
    memcpy(&Dataup[2], &data, 4);
}
void PacketOut::setThrusterStatus(uint32_t data){
    memcpy(&Dataup[1], &data, 1);
}
void PacketOut::setTemp(uint32_t data){
    memcpy(&Dataup[4], &data, 4);
}
void PacketOut::setIMUA(float data){
    memcpy(&Dataup[8], &data, 4);
}
void PacketOut::setIMUB(float data){
    memcpy(&Dataup[10], &data, 4);
}
void PacketOut::setIMUC(float data){
    memcpy(&Dataup[12], &data, 4);
}
void PacketOut::setIMUD(float data){
    memcpy(&Dataup[14], &data, 4);
}
void PacketOut::setIMUE(float data){
    memcpy(&Dataup[16], &data, 4);
}
void PacketOut::setIMUF(float data){
    memcpy(&Dataup[18], &data, 4);
}
void PacketOut::setIMUG(uint32_t data){
    memcpy(&Dataup[20], &data, 2);
}
void PacketOut::setIMUH(uint32_t data){
    memcpy(&Dataup[22], &data, 2);
}
void PacketOut::setIMUI(uint32_t data){
    memcpy(&Dataup[24], &data, 2);
}
//make array function in class adds data values to array
/*void PacketOut::setData(int Name,uint32_t data) {
    //add data to packet using #define values as indexes, eg: Name=='x', where x is a # define
    if (Name==Temp){
        memcpy(&Dataup[4], &data, 4);//using memcpy instead of bit shift ,&Dataup[4] gives initial data index location, (&data, 4 specifies data array and number of bytes/indexes)

    }
    else if (Name==ThrusterStatus){
        memcpy(&Dataup[1], &data, 1);


    }
    else if (Name==Pressure){
        memcpy(&Dataup[2], &data, 1);

    }
    else if (Name==IMUA){
        memcpy(&Dataup[8], &data, 2);
    }
    else if (Name==IMUB){
        memcpy(&Dataup[10], &data, 2);

    }
    else if (Name==IMUC){
        memcpy(&Dataup[12], &data, 2);

    }
    else if (Name==IMUD){
        memcpy(&Dataup[14], &data, 2);

    }
    else if (Name==IMUE){
        memcpy(&Dataup[16], &data, 2);
    }
    else if (Name==IMUF){
        memcpy(&Dataup[18], &data, 2);

    }
    else if (Name==IMUG){
        memcpy(&Dataup[20], &data, 2);

    }
    else if (Name==IMUH){
        memcpy(&Dataup[22], &data, 2);

    }
    else if (Name==IMUI){
        memcpy(&Dataup[24], &data, 2);

    }
    else{
        //check for error

        while(1){
            //LedOn(RED);
            HAL_Delay(100);
            //LedOff(RED);

        }

    }


}*/



void PacketOut::send() {
    //send the packet to god knows where
    //LedOn(RED);//led for no reason
    Dataup[check]=checksum((char*)Dataup);

    HAL_UART_Transmit_DMA(&huart3, Dataup, SERIAL_OUT_BUFFER_SIZE);
}



