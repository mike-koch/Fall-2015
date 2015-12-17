#include <iostream>
#include "CRC.h"

// CRC-ANSI: x^16 + x^15 + x^2 + 1
#define CRC_POLY 0x8005


uint16_t get_crc_16(const unsigned char *message_to_transmit, int length_of_message) {
    uint16_t crc = 0;
    uint16_t poly = CRC_POLY;

    for (int dataByte = 0; dataByte < length_of_message; dataByte++) {
        crc ^= message_to_transmit[dataByte];

        for (int i = 0; i < 8; i++) {
            int top_bit = crc & 1;
            crc >>= 1;
            if (top_bit) {
                crc ^= poly;
            }
        }
    }
    return crc;
}