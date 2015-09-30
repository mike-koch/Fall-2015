//
// Created by mkoch on 9/29/15.
//
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include "TransmissionUtils.h"
#include "../enum/SendMode.h"
#include "../enum/ErrorCodes.h"
#define DEBUG
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
        parity_bit ^= (character << i);
    }

    if (parity_bit == 1) {
        character |= (1 << 7);
    }
#ifdef DEBUG
    std::cout << "After parity (int): " << (int)character << std::endl;
#endif
}

void send(Frame *frame_to_send, SendMode send_mode) {
    if (send_mode == SendMode::CONSOLE) {
#ifndef DEBUG
        std::cout << "ERROR: Console is not allowed unless in DEBUG mode";
        exit(ERROR_CONSOLE_NOT_ALLOWED_UNLESS_IN_DEBUG);
#endif
        // 1st SYN character: add parity bit
        // 2nd SYN character: add parity bit
        // LENGTH  character: add parity bit
        // data: For each byte, add parity bit.
        // output bit representation of SYN (1), SYN (2), LENGTH, and each byte in data
        append_parity_bit(frame_to_send->first_syn);
        append_parity_bit(frame_to_send->second_syn);
        append_parity_bit(frame_to_send->length);
        for (int i = 0; i < frame_to_send->length; i++) {
            append_parity_bit(frame_to_send->data[i]);
        }
    } else {
        // TODO
    }
}