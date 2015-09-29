#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "app/FileManager.h"

using namespace std;

void error(char* msg);
void check_args(int argc);
void setup_socket(char *const *argv, int &sockfd, int &newsockfd, int &portno, int &clilen, sockaddr_in &serv_addr,
                  sockaddr_in &cli_addr);

int main1(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    check_args(argc);

    setup_socket(argv, sockfd, newsockfd, portno, clilen, serv_addr, cli_addr);

    // At this point, setup_socket will be waiting for the client to connect, so nothing will execute below here until
    //     the client has connected.  Once the client connects, we can start transmitting.

    // For now, we'll just transmit a sample file. No need for anything fancy
    const char* message =
            retrieve_file_to_transmit("/home/mkoch/ClionProjects/Transmitter-Receiver/Transmitter/sample-input.txt");
    cout << message << endl;

    n = write(newsockfd,message,18);
    if( n < 0 )
        error("ERROR writing to socket");
    return 0;
}

void setup_socket(char *const *argv, int &sockfd, int &newsockfd, int &portno, int &clilen, sockaddr_in &serv_addr,
                  sockaddr_in &cli_addr) {
    fprintf(stdout, "Run client by providing host and port\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cli_addr,
                       (socklen_t *) &clilen);
    if( newsockfd < 0 )
        error("ERROR on accept");
}

void error( char *msg ) {
    perror(msg);
    exit(1);
}

void check_args(int argc) {
    if( argc < 2 )
    {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
}