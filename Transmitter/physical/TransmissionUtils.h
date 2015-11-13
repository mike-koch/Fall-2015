#ifndef TRANSMITTER_BINARYCALCULATOR_H
#define TRANSMITTER_BINARYCALCULATOR_H

#include "../datalink/Frame.h"
#include "../enum/SendMode.h"
#include "../enum/ErrorCorrection.h"

char* append_char_to_output(char *binary_value, char character, unsigned int starting_offset, bool should_clobber, int frame_number);
void append_parity_bit(char &character);
void send(Frame *frame_to_send, SendMode send_mode, int newsockfd, ErrorCorrection error_correction_mode, int number_of_bits_to_clobber, int frame_number);
#endif //TRANSMITTER_BINARYCALCULATOR_H
