//
// Created by mkoch on 10/22/15.
//

#ifndef TRANSMITTER_CRCCHECK_H
#define TRANSMITTER_CRCCHECK_H

// CRC-ANSI: x^16 + x^15 + x^2 + 1
#define CRC_POLY 0x8005

char* get_crc_16(char *message_to_transmit, int length_of_message);

#endif //TRANSMITTER_CRCCHECK_H
