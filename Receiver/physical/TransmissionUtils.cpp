#include "TransmissionUtils.h"
#include "../enum/ErrorCodes.h"
void check_for_socket_error(int n);
char get_parsed_character(char *character);

void read(int sockfd) {
    int length = read_for_length(sockfd);

    int length_of_buffer = length * 8;
    char buffer[length_of_buffer];
    read(sockfd, buffer, length_of_buffer);
#ifdef DEBUG
    for (int i = 0; i < length_of_buffer; i++) {
        if (i != 0 && i % 8 == 0) {
            std::cout << "_";
        }
        std::cout << (int)(uint8_t)buffer[i];
    }
    std::cout << std::endl;
#endif
    output_buffer(buffer, length_of_buffer);
}

// Read the message received. This will look for two SYN characters, and then will return the next character,
//    which is the length.
int read_for_length(int sockfd) {
    char buffer[8];
    int n;
    n = read(sockfd, buffer, 8);
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
        n = read(sockfd, buffer, 8);
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
            n = read(sockfd, buffer, 8);
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
}

void check_for_socket_error(int n) {
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

void output_buffer(char *buffer, int length_of_buffer) {
    //    1. Call get_parsed_character with the corresponding offset
    //    2. Strip the parity bit via strip_parity_bit
    //    3. Output to the console
    for (int i = 0; i < length_of_buffer; i += 8) {
        char next_byte[8];
        get_next_eight_bits(buffer, next_byte, i);

        char parsed_character = get_parsed_character(next_byte);
        strip_parity_bit(parsed_character);
        std::cout << parsed_character;
    }
}

void get_next_eight_bits(char *large_buffer, char *small_buffer, int start_value) {
    for (int i = 0; i < 8; i++) {
        small_buffer[i] = large_buffer[i + start_value];
    }
}
