//
// Created by mkoch on 11/24/15.
//

#include "HDB3Encoder.h"

void apply_hdb3(char *bit_string, int length) {
    // Starting with +, all 1s become + and -, alternating.
    char symbol = '+';
    for (int i = 0; i < length; i++) {
        if (bit_string[i] == '1') {
            bit_string[i] = symbol;
            symbol = symbol == '+'
                     ? '-'
                     : '+';
        }
    }

    /*
     *  Now proceed with the zeroes.
     *  1. Look for four zeroes.
     *  2. Once four are found, count the number of pluses/minuses since the last violation
     *      2a. If the # of violations is odd, use 000V
     *      2b. If the # of violations is even, use B00V
     *  3. Check for the previous pulse.
     *      3a. If the pattern is 000V, V is the same as the previous pulse.
     *      3b. If the pattern is B00V, B and V are the opposite of the previous pulse.
     */
    int last_violation_position = -1;
    char last_violation = '-'; // The first violation will always be a '-'
    int number_of_zeroes = 0;

    for (int i = 0; i < length; i++) {
        if (bit_string[i] == '0') {
            if (number_of_zeroes == 3) {
                // We are at the fourth zero.
                int number_of_pulses_since_last_violation = 0;

                for (int j = last_violation_position+1; j < i; j++) {
                    if (bit_string[j] == '+' || bit_string[j] == '-') {
                        last_violation = bit_string[j];
                        number_of_pulses_since_last_violation++;
                    }
                }

                if (number_of_pulses_since_last_violation % 2 == 0) {
                    // Use the B00V pattern
                    char pulse = last_violation == '-'
                                     ? '+'
                                     : '-';
                    bit_string[i] = pulse;
                    bit_string[i - 3] = pulse;
                    last_violation = pulse;
                    last_violation_position = i;
                } else {
                    // Use the 000V pattern
                    bit_string[i] = last_violation;
                    last_violation_position = i;
                }


                number_of_zeroes = 0;
            } else {
                number_of_zeroes++;
            }
        } else {
            number_of_zeroes = 0;
        }
    }
}