#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include "physical/ReceivingUtils.h"
#include "enum/ErrorCorrection.h"

void check_args(int argc);
void setup_socket(char *const *argv, int &newsockfd);
void error(const char *msg) {
    perror(msg);
    exit(0);
}
bool socket_is_alive(int sockfd);

int main(int argc, char *argv[])
{
    ErrorCorrection error_correction_mode = ErrorCorrection::CRC;
    int newsockfd;

    check_args(argc);
    setup_socket(argv, newsockfd);

    // At this point, setup_socket will be waiting for the client to connect, so nothing will execute below here until
    //     the client has connected.  Once the client connects, we can start transmitting.

    while (socket_is_alive(newsockfd)) {
        read(newsockfd, error_correction_mode);
    }

    return 0;
}

bool socket_is_alive(int sockfd) {
    char buffer[1];
    ssize_t received = recv(sockfd, buffer, 1, MSG_PEEK);
    if (received < 0) {
        error("An error occurred when trying to check if the socket was still alive.");
    }
    return (bool)received;
}


void check_args(int argc) {
    if( argc < 2 ) {
        std::cerr << "ERROR: No port provided!" << std::endl;
        std::cout << "Usage: Receiver [port]\nNote: If you wish to output to a file, you should pipe Receiver's output to your file." << std::endl;
        exit(1);
    }
}

void setup_socket(char *const *argv, int &newsockfd) {
    struct sockaddr_in serv_addr, cli_addr;

    fprintf(stderr, "Run client by providing host and port\n----------------------------------------\n");
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons((uint16_t) atoi(argv[1]));
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