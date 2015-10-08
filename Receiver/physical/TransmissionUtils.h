#ifndef RECEIVER_TRANSMISSIONUTILS_H
#define RECEIVER_TRANSMISSIONUTILS_H
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

void read(int sockfd);
int read_for_length(int sockfd);
void strip_parity_bit(char &character);
void output_buffer(char *buffer, int length_of_buffer);
void get_next_eight_bits(char *large_buffer, char *small_buffer, int start_value);
char get_parsed_character(char *character);
#endif //RECEIVER_TRANSMISSIONUTILS_H
