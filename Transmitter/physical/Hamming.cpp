#include "Hamming.h"

// Takes any char with a value between '0'-'9' and returns the integer value
int get_int_value_of_char(char value) {
    return value - 48;
}

char get_value(unsigned char *message, int index, int offset);

/* original_message must be the message, treated as bytes.
 * length_of_message will be overwritten with the new char array's length.
 */
char* apply_hamming(unsigned char *original_message, unsigned int length_of_message, char *new_data) {
    // Iterate through each byte of the message and apply Hamming
    int k = 0;
    for (int i = 0; i < length_of_message; i++, k += 12) {
        char temp[12];
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
        temp[2] = get_value(original_message, i, 0);
        temp[4] = get_value(original_message, i, 1);
        temp[5] = get_value(original_message, i, 2);
        temp[6] = get_value(original_message, i, 3);
        temp[8] = get_value(original_message, i, 4);
        temp[9] = get_value(original_message, i, 5);
        temp[10] = get_value(original_message, i, 6);
        temp[11] = get_value(original_message, i, 7);

        // Now calculate and place the parity bits (all odd parity)
        // Parity bit #1 (position 1) calculates values at 1, 3, 5, 7, 9, 11
        int value = 1;
        value ^= get_int_value_of_char(temp[2]);
        value ^= get_int_value_of_char(temp[4]);
        value ^= get_int_value_of_char(temp[6]);
        value ^= get_int_value_of_char(temp[8]);
        value ^= get_int_value_of_char(temp[10]);
        temp[0] = value + '0';

        // Parity bit #2 (position 2) calculates values at 2, 3, 6, 7, 10, 11
        value = 1;
        value ^= get_int_value_of_char(temp[2]);
        value ^= get_int_value_of_char(temp[5]);
        value ^= get_int_value_of_char(temp[6]);
        value ^= get_int_value_of_char(temp[9]);
        value ^= get_int_value_of_char(temp[10]);
        temp[1] = value + '0';

        // Parity bit #3 (position 4) calculates values at 4, 5, 6, 7, 12
        value = 1;
        value ^= get_int_value_of_char(temp[4]);
        value ^= get_int_value_of_char(temp[5]);
        value ^= get_int_value_of_char(temp[6]);
        value ^= get_int_value_of_char(temp[11]);
        temp[3] = value + '0';

        // Parity bit #4 (position 8) calculates values at 8, 9, 10, 11, 12
        value = 1;
        value ^= get_int_value_of_char(temp[8]);
        value ^= get_int_value_of_char(temp[9]);
        value ^= get_int_value_of_char(temp[10]);
        value ^= get_int_value_of_char(temp[11]);
        temp[7] = value + '0';

        // The bits are still in MSB to LSB, so flip them
        new_data[k] = temp[11];
        new_data[k + 1] = temp[10];
        new_data[k + 2] = temp[9];
        new_data[k + 3] = temp[8];
        new_data[k + 4] = temp[7];
        new_data[k + 5] = temp[6];
        new_data[k + 6] = temp[5];
        new_data[k + 7] = temp[4];
        new_data[k + 8] = temp[3];
        new_data[k + 9] = temp[2];
        new_data[k + 10] = temp[1];
        new_data[k + 11] = temp[0];
    }

    return new_data;
}

char get_value(unsigned char *message, int index, int offset) {
    return ((message[index] >> offset) & 1) + '0';
}