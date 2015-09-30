//
// Created by mkoch on 9/29/15.
//

#ifndef TRANSMITTER_BINARYCALCULATOR_H
#define TRANSMITTER_BINARYCALCULATOR_H

#include "../datalink/Frame.h"
#include "../enum/SendMode.h"

char* convert_char_to_binary(char *binary_value, char character);
void append_parity_bit(char &character);
void send(Frame *frame_to_send, SendMode send_mode);
#endif //TRANSMITTER_BINARYCALCULATOR_H
