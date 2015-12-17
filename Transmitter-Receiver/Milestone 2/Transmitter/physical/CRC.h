#ifndef TRANSMITTER_CRC_H
#define TRANSMITTER_CRC_H


#include <stdint.h>

uint16_t get_crc_16(const unsigned char *message_to_transmit, int length_of_message);

#endif //TRANSMITTER_CRC_H
