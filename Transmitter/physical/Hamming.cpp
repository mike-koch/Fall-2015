#include "Hamming.h"

// Takes any char with a value between '0'-'9' and returns the integer value
int get_int_value_of_char(char value) {
    return value - 48;
}

char get_value(char *message, int index, int offset);

/* original_message must be the message, treated as bytes.
 * length_of_message will be overwritten with the new char array's length.
 */
char* apply_hamming(char *original_message, unsigned int length_of_message, char *new_data) {
    const int NUMBER_OF_BITS_PER_BYTE = 8;
    const int NUMBER_OF_PARITY_BITS_PER_BYTE = 4;

    int original_length = length_of_message;

    // For each 8 bits, we need 4 parity bits. So take length_of_message / 8 * 4 to get number of parity bits.
    // Add that to the original length to get the new length.
    length_of_message *= 12;

    // Iterate through each byte of the message and apply Hamming
    int k = 0;
    for (int i = 0; i < original_length; i++, k += 12) {
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
        new_data[k + 2] = get_value(original_message, i, 0);
        new_data[k + 4] = get_value(original_message, i, 1);
        new_data[k + 5] = get_value(original_message, i, 2);
        new_data[k + 6] = get_value(original_message, i, 3);
        new_data[k + 8] = get_value(original_message, i, 4);
        new_data[k + 9] = get_value(original_message, i, 5);
        new_data[k + 10] = get_value(original_message, i, 6);
        new_data[k + 11] = get_value(original_message, i, 7);

        // Now calculate and place the parity bits (all odd parity)
        // Parity bit #1 (position 1) calculates values at 1, 3, 5, 7, 9, 11
        int value = 1;
        value ^= get_int_value_of_char(new_data[k + 2]);
        value ^= get_int_value_of_char(new_data[k + 4]);
        value ^= get_int_value_of_char(new_data[k + 6]);
        value ^= get_int_value_of_char(new_data[k + 8]);
        value ^= get_int_value_of_char(new_data[k + 10]);
        new_data[k] = value + '0';

        // Parity bit #2 (position 2) calculates values at 2, 3, 6, 7, 10, 11
        value = 1;
        value ^= get_int_value_of_char(new_data[k + 2]);
        value ^= get_int_value_of_char(new_data[k + 5]);
        value ^= get_int_value_of_char(new_data[k + 9]);
        value ^= get_int_value_of_char(new_data[k + 10]);
        new_data[k + 1] = value + '0';

        // Parity bit #3 (position 4) calculates values at 4, 5, 6, 7, 12
        value = 1;
        value ^= get_int_value_of_char(new_data[k + 4]);
        value ^= get_int_value_of_char(new_data[k + 5]);
        value ^= get_int_value_of_char(new_data[k + 6]);
        value ^= get_int_value_of_char(new_data[k + 11]);
        new_data[k + 3] = value + '0';

        // Parity bit #4 (position 8) calculates values at 8, 9, 10, 11, 12
        value = 1;
        value ^= get_int_value_of_char(new_data[k + 8]);
        value ^= get_int_value_of_char(new_data[k + 9]);
        value ^= get_int_value_of_char(new_data[k + 10]);
        value ^= get_int_value_of_char(new_data[k + 11]);
        new_data[k + 7] = value + '0';
    }

    return new_data;
}

char get_value(char *message, int index, int offset) {
    return ((message[index] >> offset) & 1) + '0';
}