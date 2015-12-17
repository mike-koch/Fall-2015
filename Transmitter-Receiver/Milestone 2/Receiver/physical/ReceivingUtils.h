#ifndef RECEIVER_RECEIVINGUTILS_H
#define RECEIVER_RECEIVINGUTILS_H
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include "../enum/ErrorCorrection.h"

void read(int sockfd, ErrorCorrection error_correction_mode);
int read_for_length(int sockfd, ErrorCorrection error_correction_mode);
void strip_parity_bit(char &character);
void get_next_eight_bits(char *large_buffer, char *small_buffer, int start_value);
char get_parsed_character(char *character);
#endif //RECEIVER_TRANSMISSIONUTILS_H
