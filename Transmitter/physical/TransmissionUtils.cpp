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