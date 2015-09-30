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

int check_bit(char &character, int position);

/**
 * Converts a single character into its binary value, stored in a char array. The array must be of size 8.
 *
 * **NOTE: The binary value is order from least-significant bit to most-significant, going left-to-right!**
 */
char* convert_char_to_binary(char *binary_value, char character) {
    for (int i = 0; i < 8; ++i) {
        int binary_value_as_int = (character >> i & 1);
        binary_value[i] = (char)binary_value_as_int;
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

int check_bit(char &character, int position) {
    return character & (1<<(position));
}

void send(Frame *frame_to_send, SendMode send_mode, int &newsockfd) {
    if (send_mode == SendMode::CONSOLE) {
#ifndef DEBUG
        std::cout << "ERROR: Console is not allowed unless in DEBUG mode";
        exit(ERROR_CONSOLE_NOT_ALLOWED_UNLESS_IN_DEBUG);
#endif
        append_parity_bit(frame_to_send->first_syn);
        append_parity_bit(frame_to_send->second_syn);
        append_parity_bit(frame_to_send->length);
        for (int i = 0; i < strlen(frame_to_send->data); i++) {
            append_parity_bit(frame_to_send->data[i]);
        }
    } else {
        // TODO
    }
}