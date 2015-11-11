//#define DEBUG
#include "ReceivingUtils.h"
#include "../enum/ErrorCodes.h"
#include "../datalink/Framing.h"
#include "CRC.h"

void check_for_socket_error(ssize_t n);
void output_frame_contents(Frame *frame);

void read(int sockfd, ErrorCorrection error_correction_mode) {
    int length = read_for_length(sockfd);
    int current_bytes_read = 0;

    int multiplier = error_correction_mode == ErrorCorrection::HAMMING ? 12 : 8;
    int length_of_buffer = length * multiplier;
    char buffer[length_of_buffer];

    while (true) {
        current_bytes_read += read(sockfd, buffer + current_bytes_read, (size_t)length_of_buffer - current_bytes_read);
        if (current_bytes_read == length_of_buffer) {
            break;
        }
    }

#ifdef DEBUG
    for (int i = 0; i < length_of_buffer; i++) {
        if (i != 0 && i % 8 == 0) {
            std::cout << "_";
        }
        std::cout << (int)(uint8_t)buffer[i];
    }
    std::cout << std::endl;
#endif


    Frame *frame = new Frame();
    bool build_result = build_frame(frame, length, buffer, error_correction_mode);
    if (build_result) {
        output_frame_contents(frame);
    }
    delete frame;
}

// Read the message received. This will look for two SYN characters, and then will return the next character,
//    which is the length.
int read_for_length(int sockfd) {
    char buffer[8];
    ssize_t n;
    n = read(sockfd, buffer, (size_t)8);
    check_for_socket_error(n);
#ifdef DEBUG
    for (int i = 0; i < 8; i++) {
        std::cout << (int)(uint8_t)buffer[i];
    }
    std::cout << "|";
#endif
    char character = get_parsed_character(buffer);
    if (character == 22) {
        // Get the next character. If it's also a SYN character, the next must be the length
        while (true) {
            n = read(sockfd, buffer, (size_t)8);
            check_for_socket_error(n);
            if (n > 0) {
                break;
            }
        }
#ifdef DEBUG
        for (int i = 0; i < 8; i++) {
            std::cout << (int)(uint8_t)buffer[i];
        }
        std::cout << "|";
#endif
        character = get_parsed_character(buffer);
        if (character == 22) {
            // Next is the length
            while (true) {
                n = read(sockfd, buffer, (size_t)8);
                check_for_socket_error(n);
                if (n > 0) {
                    break;
                }
            }
#ifdef DEBUG
            for (int i = 0; i < 8; i++) {
                std::cout << (int)(uint8_t)buffer[i];
            }
            std::cout << "|";
#endif
            character = get_parsed_character(buffer);
            return character;
        } else {
            std::cerr << "ERROR: Received first SYN, but second character was not SYN. Failing frame is below:" << std::endl;
            for (int i = 0; i < 8; i++) {
            std::cerr << buffer[i];
        }
        std::cerr << std::endl;
        }
    } else {
        std::cerr << "ERROR: Expected first SYN, but character was not SYN. Failing frame is below:" << std::endl;
        for (int i = 0; i < 8; i++) {
            std::cerr << buffer[i];
        }
        std::cerr << std::endl;
    }

    exit(ERROR_MALFORMED_CONTENT);
    return -1;
}

void check_for_socket_error(ssize_t n) {
    if (n < 0) {
        perror("ERROR: Unable to read from socket");
        exit(ERROR_UNABLE_TO_READ_FROM_SOCKET);
    }
}

char get_parsed_character(char *character) {
    char parsed_character = 0;
    for (int i = 0; i < 8; i++) {
        if (character[i] == '1') {
            parsed_character |= (1 << i);
        } else {
            parsed_character &= ~(1 << i);
        }
    }

    return parsed_character;
}

void strip_parity_bit(char &character) {
    character &= ~(1 << 7);
}

void output_frame_contents(Frame *frame) {
    for (int i = 0; i < frame->length; i++) {
        std::cout << frame->data[i];
    }
}

void get_next_eight_bits(char *large_buffer, char *small_buffer, int start_value) {
    for (int i = 0; i < 8; i++) {
        small_buffer[i] = large_buffer[i + start_value];
    }
}
