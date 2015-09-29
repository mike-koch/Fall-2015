//
// Created by mkoch on 9/29/15.
//

#ifndef TRANSMITTER_BINARYCALCULATOR_H
#define TRANSMITTER_BINARYCALCULATOR_H

#include "Frame.h"

char* convert_char_to_binary(char *binary_value, char character);
int calculate_parity_bit(char *binary_value);
Frame* build_frames(char *data, int offset, Frame *frame_to_populate);
#endif //TRANSMITTER_BINARYCALCULATOR_H
