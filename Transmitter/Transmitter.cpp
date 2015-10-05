#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "app/FileManager.h"
#include "datalink/Framing.h"
#include "physical/TransmissionUtils.h"

using namespace std;

void error(char* msg);
void check_args(int argc);
void setup_socket(char *const *argv, int &newsockfd);

int main(int argc, char *argv[])
{
    int newsockfd;

    check_args(argc);

    setup_socket(argv, newsockfd);

    // At this point, setup_socket will be waiting for the client to connect, so nothing will execute below here until
    //     the client has connected.  Once the client connects, we can start transmitting.

    // For now, we'll just transmit a sample file. No need for anything fancy
    std::string message =
            retrieve_file_to_transmit("/home/mkoch/ClionProjects/Transmitter-Receiver/Transmitter/lorem-ipsum.txt");
    for (unsigned int i = 0; i < strlen(message.c_str()); i += 64) {
        Frame *frame = new Frame();
        build_frame(message.c_str(), i, frame);
        send(frame, SendMode::SOCKET, newsockfd);
        delete frame;
    }

    close(newsockfd);
    
    return 0;
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

void error(char *msg) {
    perror(msg);
    exit(1);
}

void check_args(int argc) {
    if( argc < 2 ) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
}