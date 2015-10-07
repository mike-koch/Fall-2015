#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include "physical/TransmissionUtils.h"

void check_args(int argc);
void setup_socket(char *const *argv, int &newsockfd);
void error(char *msg) {
    perror(msg);
    exit(0);
}
bool socket_is_alive(int sockfd);

int main(int argc, char *argv[])
{
    int newsockfd;

    check_args(argc);
    setup_socket(argv, newsockfd);

    // At this point, setup_socket will be waiting for the client to connect, so nothing will execute below here until
    //     the client has connected.  Once the client connects, we can start transmitting.

    while (socket_is_alive(newsockfd)) {
        read(newsockfd);
    }

    return 0;
}

bool socket_is_alive(int sockfd) {
    char buffer[1];
    ssize_t received = recv(sockfd, buffer, 1, MSG_PEEK);
    if (received < 0) {
        error("ERROR");
    }
    return received;
}


void check_args(int argc) {
    if( argc < 2 ) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
}

void setup_socket(char *const *argv, int &newsockfd) {
    struct sockaddr_in serv_addr, cli_addr;

    fprintf(stdout, "Run client by providing host and port\n");
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }
    listen(sockfd,5);
    int clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cli_addr,
                       (socklen_t *) &clilen);
    if( newsockfd < 0 ) {
        error("ERROR on accept");
    }
}