//#define DEBUG
#include <string.h>
#include <iostream>
#include <bitset>
#include <unistd.h>
#include "TransmissionUtils.h"
#include "../enum/ErrorCodes.h"
#include "HDB3Encoder.h"
#include <fstream>
#ifndef DEBUG
#include <stdlib.h>
#endif

bool check_bit(char &character, int position);

void append_parity_bits(Frame *frame_to_send, unsigned int data_length);

unsigned int get_output_size(unsigned int data_length);

void output_to_console(char message[], unsigned int length);

void send_through_socket(char message[], unsigned int size, int newsockfd);

/**
 * Converts a single character into its binary value and appended to the output.
 *
 * **NOTE: The binary value is order from least-significant bit to most-significant, going left-to-right!**
 */
char* append_char_to_output(char *binary_value, char character, unsigned int starting_offset) {
    for (int i = 0; i < 8; ++i) {
        int binary_value_as_int = (character >> i & 1);
        binary_value[i + starting_offset] = binary_value_as_int + '0';
    }
    return binary_value;
}

void append_parity_bit(char &character) {
#ifdef DEBUG
    //std::cout << "Before parity (int): " << (int)character << std::endl;
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

// Simle check to see if a bit is set by ANDing it with 1
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

    // Append the parity bits to each character for transmission. This honestly has no utilization besides appending
    //    it to each character. It is simply stripped by the receiver.
    unsigned int data_length = frame_to_send->length;
    append_parity_bits(frame_to_send, frame_to_send->length);

    // Add each byte to the output message (SYN, SYN, length)
    char output_message[get_output_size(data_length)];
    append_char_to_output(output_message, frame_to_send->first_syn, 0);
    append_char_to_output(output_message, frame_to_send->second_syn, 8);
    append_char_to_output(output_message, frame_to_send->length, 16);

    // Get the encoded bit string of 1's and 0's so we can encode the message using HDB3
    char message[get_output_size(data_length) - 24];
    for (int i = 0; i < data_length; i++) {
#ifdef DEBUG
        append_char_to_output(output_message, frame_to_send->data[i], 24 + (8 * i));
#endif
        append_char_to_output(message, frame_to_send->data[i], 8 * i);
    }

#ifdef DEBUG
    output_to_console(output_message, get_output_size(data_length));
#endif

    apply_hdb3(message, get_output_size(data_length) - 24);

    // Now that the message is encoded, add it to the final message to send to the receiver
    for (int i = 24; i < get_output_size(data_length); i++) {
        output_message[i] = message[i - 24];
    }

    // Output to the proper end, either the console or the socket.
    if (send_mode == SendMode::CONSOLE) {
        output_to_console(output_message, get_output_size(data_length));
    } else {
        send_through_socket(output_message, get_output_size(data_length), newsockfd);
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
        if (i == 8 || i == 16 || i == 24) {
            std::cout << "|";
        } else if (i % 8 == 0 && i != 0) {
            std::cout << "_";
        }
        std::cout << message[i];
    }
    std::cout << std::endl;
}

void send_through_socket(char message[], unsigned int size, int newsockfd) {
#ifdef DEBUG
    output_to_console(message, size);
#endif
    int n = write(newsockfd, message, size);
    if (n < 0) {
        perror("ERROR: Unable to write to socket");
        exit(ERROR_UNABLE_TO_WRITE_TO_SOCKET);
    }
}