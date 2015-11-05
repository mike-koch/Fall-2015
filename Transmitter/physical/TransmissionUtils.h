#ifndef TRANSMITTER_BINARYCALCULATOR_H
#define TRANSMITTER_BINARYCALCULATOR_H

#include "../datalink/Frame.h"
#include "../enum/SendMode.h"
#include "../enum/ErrorCorrection.h"

char* append_char_to_output(char *binary_value, char character, unsigned int starting_offset);
void append_parity_bit(char &character);
void send(Frame *frame_to_send, SendMode send_mode, int newsockfd, ErrorCorrection error_correction_mode);
#endif //TRANSMITTER_BINARYCALCULATOR_H
