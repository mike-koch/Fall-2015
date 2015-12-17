//
// Created by mkoch on 10/7/15.
//

#include "Framing.h"
#include "../physical/CRC.h"
#include "../physical/Hamming.h"

bool build_frame(Frame *frame, int length, char *data, ErrorCorrection error_correction_mode) {
    frame->length = (char)length;

    char parsed_data[length];
    if (error_correction_mode == ErrorCorrection::HAMMING) {
        reverse_hamming(data, length, parsed_data);
    } else {
        for (int i = 0; i < length; i++) {
            char next_eight[8];
            get_next_eight_bits(data, next_eight, i * 8);
            char parsed_character = get_parsed_character(next_eight);
            parsed_data[i] = parsed_character;
        }
    }


    if (error_correction_mode == ErrorCorrection::CRC) {
        uint16_t crc = get_crc_16((unsigned char *) parsed_data, length);
        if (crc != 0) {
            std::cout << ">>> ERROR: CRC was not zero. Skipping frame. <<<" << std::endl;
            return false;
        }

        length -= 2;
        frame->length -= 2;
    }

    for (int i = 0; i < length; i++) {
        strip_parity_bit(parsed_data[i]);
    }

    strcpy(frame->data, (const char *) parsed_data);

    return true;
}