//
// Created by mkoch on 9/29/15.
//

#include <stdlib.h>
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

int calculate_parity_bit(char *binary_value) {
    // The parity bit is calculated via a simple XOR across all of the bits, followed by swapping the 1 to a 0
    // (or vice-versa).
    int parity_bit = 0;
    for (int i = 0; i < 8; i++) {
        parity_bit ^= binary_value[i];
    }

    return !parity_bit;
}