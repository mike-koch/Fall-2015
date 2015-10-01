//
// Created by mkoch on 10/1/15.
//

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

char* read(int sockfd);
int read_for_length(int sockfd);
void strip_parity_bit(char &character);
#endif //RECEIVER_TRANSMISSIONUTILS_H
