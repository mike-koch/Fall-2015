//
// Created by mkoch on 9/29/15.
//
#include "TransmissionUtils.h"

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
//    in, but populated.
Frame* build_frame(char *data, int offset, Frame *frame_to_populate) {


    return frame_to_populate;
    //--
}