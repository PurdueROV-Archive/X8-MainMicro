//
// Created by stewa on 2/6/2016.
//

#ifndef X8_MAINMICRO_PACKETOUT_H
#define X8_MAINMICRO_PACKETOUT_H

#define SERIAL_OUT_BUFFER_SIZE  28

class PacketOut {
public:



private:

    char checksum(char *bytes);

};


#endif //X8_MAINMICRO_PACKETOUT_H
