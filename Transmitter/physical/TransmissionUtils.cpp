#define DEBUG
#include <string.h>
#include <iostream>
#include <bitset>
#include <unistd.h>
#include "TransmissionUtils.h"
#include "../enum/ErrorCodes.h"
#include "CRC.h"
#include "Hamming.h"
#include <fstream>
#ifndef DEBUG
#include <stdlib.h>
#endif

bool check_bit(char &character, int position);

void append_parity_bits(Frame *frame_to_send, unsigned int data_length);

unsigned int get_output_size(unsigned int data_length, ErrorCorrection error_correction_mode);

void output_to_console(char message[], unsigned int length, ErrorCorrection error_correction_mode);

void send_through_socket(char message[], unsigned int size, int newsockfd, ErrorCorrection error_correction_mode);

char get_possibly_clobbered_bit(char character, int &number_of_clobbered_bits, int number_of_bits_to_clobber, bool &byte_clobbered, int current_byte, int current_bit);

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
    // The parity bit is calculated via a simple XOR across all of the bits, followed by swapping the 1 to a 0
    // (or vice-versa).
    int parity_bit = 1;
    for (int i = 0; i < 7; i++) {
        parity_bit ^= check_bit(character, i);
    }

    if (parity_bit == 1) {
        character |= (1 << 7);
    }

    return;
}

bool check_bit(char &character, int position) {
    return (bool) (character & (1<<(position)));
}

void send(Frame *frame_to_send, SendMode send_mode, int newsockfd, ErrorCorrection error_correction_mode, int number_of_bits_to_clobber) {
#ifndef DEBUG
    if (send_mode == SendMode::CONSOLE) {
        std::cout << "ERROR: Console is not allowed unless in DEBUG mode";
        exit(ERROR_CONSOLE_NOT_ALLOWED_UNLESS_IN_DEBUG);
    }
#endif
    unsigned int data_length = frame_to_send->length;

    if (error_correction_mode == ErrorCorrection::CRC) {
        frame_to_send->length += 2;
    }
    append_parity_bits(frame_to_send, frame_to_send->length);


    if (error_correction_mode == ErrorCorrection::CRC) {
        uint16_t crc = get_crc_16((unsigned char*) frame_to_send->data, data_length);
        frame_to_send->data[data_length++] = crc & 0xFF;
        frame_to_send->data[data_length++] = crc >> 8;
    }

    char output_message[get_output_size(data_length, error_correction_mode)];
    append_char_to_output(output_message, frame_to_send->first_syn, 0);
    append_char_to_output(output_message, frame_to_send->second_syn, 8);
    append_char_to_output(output_message, frame_to_send->length, 16);

    if (error_correction_mode == ErrorCorrection::HAMMING) {
        char new_data[data_length * 12];
        apply_hamming((unsigned char*) frame_to_send->data, data_length, new_data);

        // Clobber as many bits per frame as the user requested. Reset the clobber flag after every 12 bits since we'll be on a new byte.
        int number_of_clobbered_bits = 0;
        int current_byte = 0;
        bool byte_clobbered = false;
        for (int i = 0; i < data_length * 12; i++) {
            if (i % 12 == 0) {
                current_byte++;
                byte_clobbered = false;
            }
            output_message[24 + i] = get_possibly_clobbered_bit(new_data[i], number_of_clobbered_bits, number_of_bits_to_clobber, byte_clobbered, current_byte, i);
        }
    } else {
        for (int i = 0; i < data_length; i++) {
            append_char_to_output(output_message, frame_to_send->data[i], 24 + (8 * i));
        }
    }

    if (send_mode == SendMode::CONSOLE) {
        // Forcing the correction mode to NONE as data_length is now 1.5 times what it used to be.
        output_to_console(output_message, get_output_size(data_length, error_correction_mode), error_correction_mode);
    } else {
        send_through_socket(output_message, get_output_size(data_length, error_correction_mode), newsockfd, error_correction_mode);
    }
}

char get_possibly_clobbered_bit(char character, int &number_of_clobbered_bits, int number_of_bits_to_clobber, bool &byte_clobbered, int current_byte, int current_bit) {
    // We don't want to clobber more than one bit per byte, or if we've reached the max per frame
    if (byte_clobbered || number_of_bits_to_clobber == number_of_clobbered_bits) {
        return character;
    }

    bool clobber = (rand() % 100) % 10 == 1;

    if (clobber) {
        char return_char = character == '0' ? '1' : '0';
        int bit_flipped = current_bit % 12 == 0 ? 12 : current_bit % 12;
        std::printf("Flipped bit #%d in byte %d from %d to %d", bit_flipped, current_byte, character - 48, return_char - 48);
        std::cout << std::endl;
        byte_clobbered = true;
        number_of_clobbered_bits++;
        return return_char;
    }

    return character;
}

void append_parity_bits(Frame *frame_to_send, unsigned int data_length) {
    append_parity_bit(frame_to_send->first_syn);
    append_parity_bit(frame_to_send->second_syn);
    append_parity_bit(frame_to_send->length);
    for (int i = 0; i < data_length; i++) {
        append_parity_bit(frame_to_send->data[i]);
    }
}

unsigned int get_output_size(unsigned int data_length, ErrorCorrection error_correction_mode) {
    const int BYTE = 8;
    const int SYN = BYTE;
    const int LENGTH = BYTE;
    const int DATA = data_length;

    // Hamming requires 1.5 times as many bit slots as CRC or no error correction
    const int MULTIPLIER = error_correction_mode == ErrorCorrection::HAMMING
                           ? 12
                           : 8;

    return SYN + SYN + LENGTH + (DATA * MULTIPLIER);
}

void output_to_console(char message[], unsigned int length, ErrorCorrection error_correction_mode) {
    for (int i = 0; i < length; i++) {
        std::cout << (int)message[i] - 48;
    }
    std::cout << std::endl;
}

void send_through_socket(char message[], unsigned int size, int newsockfd, ErrorCorrection error_correction_mode) {
#ifdef DEBUG
    output_to_console(message, size, error_correction_mode);
#endif
    int n = write(newsockfd, message, size);
    std::cout << n << std::endl;
    if (n < 0) {
        perror("ERROR: Unable to write to socket");
        exit(ERROR_UNABLE_TO_WRITE_TO_SOCKET);
    }
}