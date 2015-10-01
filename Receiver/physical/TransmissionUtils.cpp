//
// Created by mkoch on 10/1/15.
//

#include "TransmissionUtils.h"
#include "../enum/ErrorCodes.h"
void check_for_socket_error(int n);
char get_parsed_character(char *character);

char* read(int sockfd) {
    int length = read_for_length(sockfd);

    char buffer[length * 8];
    int n = read(sockfd, buffer, length * 8);
    std::cout << buffer << std::endl;

    return "A";
}

// Read the message received. This will look for two SYN characters, and then will return the next character,
//    which is the length.
int read_for_length(int sockfd) {
    char buffer[8];
    int n;
    n = read(sockfd, buffer, 8);
    check_for_socket_error(n);

    char character = get_parsed_character(buffer);
    if (character == 22) {
        // Get the next character. If it's also a SYN character, the next must be the length
        n = read(sockfd, buffer, 8);
        check_for_socket_error(n);
        character = get_parsed_character(buffer);
        if (character == 22) {
            // Next is the length
            n = read(sockfd, buffer, 8);
            check_for_socket_error(n);
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
