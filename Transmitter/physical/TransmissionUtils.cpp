//
// Created by mkoch on 9/29/15.
//
#define DEBUG
#include <string.h>
#include <iostream>
#include <bitset>
#include "TransmissionUtils.h"
#ifndef DEBUG
#include <stdlib.h>
#include "../enum/ErrorCodes.h"
#endif

bool check_bit(char &character, int position);

void append_parity_bits(Frame *frame_to_send, unsigned int data_length);

unsigned int get_output_size(unsigned int data_length);

void append_array_to_output(char output_message[], char binary_value_for_char[8], unsigned int starting_index);

void output_to_console(char message[], unsigned int length);

/**
 * Converts a single character into its binary value and appended to the output.
 *
 * **NOTE: The binary value is order from least-significant bit to most-significant, going left-to-right!**
 */
char* append_char_to_output(char *binary_value, char character, unsigned int starting_offset) {
    for (int i = 0; i < 8; ++i) {
        int binary_value_as_int = (character >> i & 1);
        binary_value[i + starting_offset] = (char)binary_value_as_int;
    }
    return binary_value;
}

void append_parity_bit(char &character) {
#ifdef DEBUG
    std::cout << "Before parity (int): " << (int)character << std::endl;
#endif
    // The parity bit is calculated via a simple XOR across all of the bits, followed by swapping the 1 to a 0
    // (or vice-versa).
    int parity_bit = 1;
    for (int i = 0; i < 7; i++) {
        parity_bit ^= check_bit(character, i);
    }

    if (parity_bit == 1) {
        character |= (1 << 7);
    }
#ifdef DEBUG
    std::cout << "After parity (int): " << (int)(uint8_t)character << std::endl;
#endif

    return;
}

bool check_bit(char &character, int position) {
    return (bool) (character & (1<<(position)));
}

void send(Frame *frame_to_send, SendMode send_mode, int newsockfd) {
#ifndef DEBUG
    if (send_mode == SendMode::CONSOLE) {
        std::cout << "ERROR: Console is not allowed unless in DEBUG mode";
        exit(ERROR_CONSOLE_NOT_ALLOWED_UNLESS_IN_DEBUG);
    }
#endif

    unsigned int data_length = frame_to_send->length;
    append_parity_bits(frame_to_send, frame_to_send->length);

    char output_message[get_output_size(data_length)];
    append_char_to_output(output_message, frame_to_send->first_syn, 0);
    append_char_to_output(output_message, frame_to_send->second_syn, 8);
    append_char_to_output(output_message, data_length, 16);

    for (int i = 0; i < data_length; i++) {
        append_char_to_output(output_message, frame_to_send->data[i], 24 + (8 * i));
    }

    if (send_mode == SendMode::CONSOLE) {
        output_to_console(output_message, get_output_size(data_length));
    }
}

void append_parity_bits(Frame *frame_to_send, unsigned int data_length) {
    append_parity_bit(frame_to_send->first_syn);
    append_parity_bit(frame_to_send->second_syn);
    append_parity_bit(frame_to_send->length);
    for (int i = 0; i < data_length; i++) {
        append_parity_bit(frame_to_send->data[i]);
    }
}

unsigned int get_output_size(unsigned int data_length) {
    const int BYTE = 8;
    const int SYN = BYTE;
    const int LENGTH = BYTE;
    const int DATA = BYTE * data_length;

    return SYN + SYN + LENGTH + DATA;
}

void output_to_console(char message[], unsigned int length) {
    for (int i = 0; i < length; i++) {
        std::cout << (int)message[i];
    }
}