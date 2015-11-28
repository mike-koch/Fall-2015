#include "HDB3Decoder.h"

void reverse_hdb3(char *bit_string, int length) {
    // Keep track of our last violation, or 'X' for none
    char last_violation = 'X';

    // String index of latest violation, or -1 for none
    int last_violation_position = -1;

    for (int i = 0; i < length; i++) {
        if (bit_string[i] == last_violation) {
            // Since we have two of the same violation, this one and the one 3 chars back are both 0.
            // It is possible that bit_string[i - 3] is already zero; however it won't hurt to explicitly set it again.
            bit_string[i] = '0';
            bit_string[i - 3] = '0';

            // Reset the last violation and position since we have decoded the latest violation.
            last_violation = 'X';
            last_violation_position = -1;
        } else {
            if (bit_string[i] == '+' || bit_string[i] == '-') {
                if (last_violation_position > -1) {
                    // The last violation and the current one are different symbols, so they are both 1s.
                    bit_string[i] = '1';
                    bit_string[last_violation_position] = '1';

                    // Reset the last violation and position since we have decoded the latest violation.
                    last_violation = 'X';
                    last_violation_position = -1;
                } else {
                    // We do not have a pair of violations, so keep track of this one.
                    last_violation = bit_string[i];
                    last_violation_position = i;
                }
            }
        }
    }

    // It is possible that we missed some violations. All of the missed ones are 1s, so replace any + or - with 1.
    for (int i = 0; i < length; i++) {
        if (bit_string[i] == '+' || bit_string[i] == '-') {
            bit_string[i] = '1';
        }
    }
}