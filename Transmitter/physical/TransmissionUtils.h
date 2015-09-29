//
// Created by mkoch on 9/29/15.
//

#ifndef TRANSMITTER_BINARYCALCULATOR_H
#define TRANSMITTER_BINARYCALCULATOR_H

#include "Frame.h"

char* convert_char_to_binary(char *binary_value, char character);
void append_parity_bit(char &character);
Frame* build_frame(char *data, unsigned int offset, Frame *frame_to_populate);
#endif //TRANSMITTER_BINARYCALCULATOR_H
