//
// Created by mkoch on 10/7/15.
//

#include "Framing.h"
void build_frame(Frame &frame, int length, char *data) {
    const char SYN = 22;

    frame.first_syn = SYN;
    frame.second_syn = SYN;
    frame.length = length;

    char parsed_data[length];
    for (int i = 0; i < length; i++) {
        char next_eight[8];
        get_next_eight_bits(data, next_eight, i * 8);
        char parsed_character = get_parsed_character(next_eight);
        strip_parity_bit(parsed_character);
        parsed_data[i] = parsed_character;
    }
    strcpy(frame.data, parsed_data);
}