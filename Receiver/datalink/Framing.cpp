//
// Created by mkoch on 10/7/15.
//

#include "Framing.h"
void build_frame(Frame *frame, int length, char *data) {
    frame->length = (char)length;

    char parsed_data[length];

    for (int i = 0; i < length; i++) {
        // Get the next byte of data, and add it to a helper array (parsed_data). Do this for everything for this frame
        char next_eight[8];
        get_next_eight_bits(data, next_eight, i * 8);
        char parsed_character = get_parsed_character(next_eight);
        strip_parity_bit(parsed_character);
        parsed_data[i] = parsed_character;
    }

    // Copy the data from parsed_data over to the frame's array
    strcpy(frame->data, parsed_data);
}