//
// Created by stewa on 2/6/2016.
//

#include "PacketOut.h"
#include "main.h"

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
