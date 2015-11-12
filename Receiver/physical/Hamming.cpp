#include "Hamming.h"
#include "ReceivingUtils.h"

// Takes any char with a value between '0'-'9' and returns the integer value
int get_int_value_of_char(char value) {
    return value - 48;
}

/*
 * message - full message bit string, each byte is in most-significant to least-significant order
 * length - length of the message
 * returned_data - pointer to place the resulting bytes in after de-hamming
 */
void reverse_hamming(char *message, int length, char *returned_data) {
    //-- Iterate over every 12 characters in the message block and de-ham.
    int returned_data_index = 0;
    for (int i = 0; i < length * 12; i += 12, returned_data_index++) {
        /*
         *  ORIGINAL BIT POSITION  |  POSITION AFTER DE-HAMMING
         *  -----------------------+----------------------------
         *  2                      |  0
         *  4                      |  1
         *  5                      |  2
         *  6                      |  3
         *  8                      |  4
         *  9                      |  5
         *  10                     |  6
         *  11                     |  7
         */
        int syndrome = check_hamming(message, i);

        if (syndrome != 0) {
            // There is an error in the message. Correct it by flipping the bit in error.
            message[(i + 1) * (syndrome - 1)] ^= 1;
        }

        // Build the byte now that we have checked hamming.
        char next_byte[8];
        next_byte[0] = message[i + 9];
        next_byte[1] = message[i + 7];
        next_byte[2] = message[i + 6];
        next_byte[3] = message[i + 5];
        next_byte[4] = message[i + 3];
        next_byte[5] = message[i + 2];
        next_byte[6] = message[i + 1];
        next_byte[7] = message[i];

        returned_data[returned_data_index] = get_parsed_character(next_byte);
    }
}

int check_hamming(char *message, int starting_index) {
    // NOTE: Since bits are transferred LSB to MSB, bit 1 is in position 12, bit 12 in position 1, etc.

    // Calculate the parity bits (all odd parity)
    // Parity bit #1 (position 1) calculates values at 1, 3, 5, 7, 9, 11. A non-zero value adds 1 to the syndrome.
    int syndrome = 0;
    int value = 1;
    value ^= get_int_value_of_char(message[11 - starting_index]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 2)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 4)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 6)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 8)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 10)]);
    if (value != 0) {
        syndrome += 1;
    }

    // Parity bit #2 (position 2) calculates values at 2, 3, 6, 7, 10, 11. A non-zero value adds 2 to the syndrome.
    value = 1;
    value ^= get_int_value_of_char(message[11 - (starting_index + 1)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 2)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 5)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 6)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 9)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 10)]);
    if (value != 0) {
        syndrome += 2;
    }

    // Parity bit #3 (position 4) calculates values at 4, 5, 6, 7, 12. A non-zero value returns 4 to the syndrome.
    value = 1;
    value ^= get_int_value_of_char(message[11 - (starting_index + 3)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 4)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 5)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 6)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 11)]);
    if (value != 0) {
        syndrome += 4;
    }

    // Parity bit #4 (position 8) calculates values at 8, 9, 10, 11, 12. A non-zero value returns 8 to the syndrome.
    value = 1;
    value ^= get_int_value_of_char(message[11 - (starting_index + 7)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 8)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 9)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 10)]);
    value ^= get_int_value_of_char(message[11 - (starting_index + 11)]);
    if (value != 0) {
        syndrome += 8;
    }

    return syndrome;
}