#include "TransmissionUtils.h"
#include "../enum/ErrorCodes.h"
#include "../datalink/Framing.h"
void check_for_socket_error(ssize_t n);
void output_frame_contents(Frame *frame);

void read(int sockfd) {
    int length = read_for_length(sockfd);

    int length_of_buffer = length * 8;
    char buffer[length_of_buffer];
    read(sockfd, buffer, (size_t)length_of_buffer);
    //std::cout << "Read the contents of the file descriptor" << std::endl;
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
    build_frame(frame, length, buffer);
    output_frame_contents(frame);
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
        n = read(sockfd, buffer, (size_t)8);
        check_for_socket_error(n);
#ifdef DEBUG
        for (int i = 0; i < 8; i++) {
            std::cout << (int)(uint8_t)buffer[i];
        }
        std::cout << "|";
#endif
        character = get_parsed_character(buffer);
        if (character == 22) {
            // Next is the length
            n = read(sockfd, buffer, (size_t)8);
            check_for_socket_error(n);
#ifdef DEBUG
            for (int i = 0; i < 8; i++) {
                std::cout << (int)(uint8_t)buffer[i];
            }
            std::cout << "|";
#endif
            character = get_parsed_character(buffer);
            return character;
        }
    }

    std::cerr << "ERROR: Receiver received malformed input. Terminating";
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
        if (character[i]) {
            parsed_character |= (1 << i);
        } else {
            parsed_character &= ~(1 << i);
        }
    }

    strip_parity_bit(parsed_character);
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
