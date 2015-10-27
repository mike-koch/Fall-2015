//
// Created by mkoch on 10/27/15.
//

#include "Hamming.h"

// Takes any char with a value between '0'-'9' and returns the integer value
int get_int_value_of_char(char value) {
    return value - 48;
}

/* original_message must be the message, already converted to a bit string of 1's and 0's.
 * length_of_message will be overwritten with the new char array's length.
 */
char* apply_hamming(char *original_message, int &length_of_message) {
    const int NUMBER_OF_BITS_PER_BYTE = 8;
    const int NUMBER_OF_PARITY_BITS_PER_BYTE = 4;

    int original_length = length_of_message;

    // For each 8 bits, we need 4 parity bits. So take length_of_message / 8 * 4 to get number of parity bits.
    // Add that to the original length to get the new length.
    length_of_message = original_length + (original_length / NUMBER_OF_BITS_PER_BYTE * NUMBER_OF_PARITY_BITS_PER_BYTE);

    // Create a new char* array with the new size;
    char new_data[length_of_message];

    // Iterate through each byte of the message and apply Hamming
    for (int i = 0; i < original_length; i += 8) {
        /*
         *  ORIGINAL BIT POSITION  |  POSITION AFTER HAMMING
         *  -----------------------+------------------------
         *  0                      |  2
         *  1                      |  4
         *  2                      |  5
         *  3                      |  6
         *  4                      |  8
         *  5                      |  9
         *  6                      | 10
         *  7                      | 11
         */
        new_data[i + 2] = original_message[i];
        new_data[i + 4] = original_message[i + 1];
        new_data[i + 5] = original_message[i + 2];
        new_data[i + 6] = original_message[i + 3];
        new_data[i + 8] = original_message[i + 4];
        new_data[i + 9] = original_message[i + 5];
        new_data[i + 10] = original_message[i + 6];
        new_data[i + 11] = original_message[i + 7];

        // Now calculate and place the parity bits (all odd parity)
        // Parity bit #1 (position 1) calculates values at 1, 3, 5, 7, 9, 11
        int value = 1;
        value ^= get_int_value_of_char(new_data[i + 2]);
        value ^= get_int_value_of_char(new_data[i + 4]);
        value ^= get_int_value_of_char(new_data[i + 6]);
        value ^= get_int_value_of_char(new_data[i + 8]);
        value ^= get_int_value_of_char(new_data[i + 10]);
        new_data[i] = (char) value + '0';

        // Parity bit #2 (position 2) calculates values at 2, 3, 6, 7, 10, 11
        value = 1;
        value ^= get_int_value_of_char(new_data[i + 2]);
        value ^= get_int_value_of_char(new_data[i + 5]);
        value ^= get_int_value_of_char(new_data[i + 6]);
        value ^= get_int_value_of_char(new_data[i + 9]);
        value ^= get_int_value_of_char(new_data[i + 10]);
        new_data[i + 1] = (char) value + '0';

        // Parity bit #3 (position 4) calculates values at 4, 5, 6, 7, 12
        value = 1;
        value ^= get_int_value_of_char(new_data[i + 4]);
        value ^= get_int_value_of_char(new_data[i + 5]);
        value ^= get_int_value_of_char(new_data[i + 6]);
        value ^= get_int_value_of_char(new_data[i + 11]);
        new_data[i + 3] = (char) value + '0';

        // Parity bit #4 (position 8) calculates values at 8, 9, 10, 11, 12
        value = 1;
        value ^= get_int_value_of_char(new_data[i + 8]);
        value ^= get_int_value_of_char(new_data[i + 9]);
        value ^= get_int_value_of_char(new_data[i + 10]);
        value ^= get_int_value_of_char(new_data[i + 11]);
        new_data[i + 7] = (char) value + '0';
    }
}