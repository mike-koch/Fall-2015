//
// Created by mkoch on 9/29/15.
//

#include "BinaryCalculator.h"

/**
 * Converts a single character into its binary value, stored in a char array.
 *
 * **NOTE: The binary value is order from least-significant bit to most-significant, going left-to-right!**
 */
char* convert_char_to_binary(char character) {
    char *binary_value = new char[8];
    for (int i = 0; i < 8; ++i) {
        int binary_value_as_int = (character >> i & 1);
        binary_value[i] = (char)binary_value_as_int;
    }
    return binary_value;
}