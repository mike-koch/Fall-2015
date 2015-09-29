//
// Created by mkoch on 9/29/15.
//
#include <string.h>
#include <iostream>
#include "TransmissionUtils.h"

int get_end_offset(char *data, unsigned int offset);

/**
 * Converts a single character into its binary value, stored in a char array. The array must be of size 8.
 *
 * **NOTE: The binary value is order from least-significant bit to most-significant, going left-to-right!**
 */
char* convert_char_to_binary(char *binary_value, char character) {
    for (int i = 0; i < 8; ++i) {
        int binary_value_as_int = (character >> i & 1);
        binary_value[i] = (char)binary_value_as_int;
    }
    return binary_value;
}

void append_parity_bit(char &character) {
    // The parity bit is calculated via a simple XOR across all of the bits, followed by swapping the 1 to a 0
    // (or vice-versa).
    int parity_bit = 1;
    for (int i = 0; i < 7; i++) {
        parity_bit ^= (character << i);
    }

    if (parity_bit == 1) {
        character |= (1 << 7);
    }
}

// Takes a char array of the data to be framed, starting at the current offset. Returns the same frame that is passed
//    in, but populated. Offset is 0-indexed.
Frame* build_frame(char *data, unsigned int offset, Frame *frame_to_populate) {
    unsigned int end_offset = get_end_offset(data, offset);
    unsigned int length = end_offset - offset;

    for (int i = 0; i < length; i++) {
        frame_to_populate->data[i] = data[offset + i];
    }

    return frame_to_populate;
}

// Helper function to get the "actual" end index, in case the next frame is going to be less than 64 bytes. Offset is 0-indexed
int get_end_offset(char *data, unsigned int offset) {
    const int offset_amount = 64;
    unsigned int length_to_process = offset + offset_amount;
    size_t length_of_data = strlen(data);

    if (length_to_process > length_of_data) {
        return length_of_data;
    }
}