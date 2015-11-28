#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <netdb.h>
#include "app/FileManager.h"
#include "datalink/Framing.h"
#include "physical/TransmissionUtils.h"

using namespace std;

void error(const char* msg);
void check_args(int argc);
void connect_to_server(char *argv[], int &sockfd);

int main(int argc, char *argv[])
{
    int sockfd;

    check_args(argc);

    connect_to_server(argv, sockfd);

    std::string message =
            retrieve_file_to_transmit(argv[3]);
    for (unsigned int i = 0; i < strlen(message.c_str()); i += 64) {
        Frame *frame = new Frame();
        build_frame(message.c_str(), i, frame);
        send(frame, SendMode::SOCKET, sockfd);
        delete frame;
    }

    close(sockfd);
    
    return 0;
}

void connect_to_server(char *argv[], int &sockfd) {
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if( sockfd < 0 )
        error("ERROR opening socket");
    hostent *server = gethostbyname(argv[1]);
    if( server == NULL )
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if( connect(sockfd, (const sockaddr *) &serv_addr,sizeof(serv_addr)) < 0 )
        error("ERROR connecting");
}

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void check_args(int argc) {
    if (argc < 4) {
        std::cerr << "Usage: Transmitter.cpp [hostname] [port] [filename]";
        exit(0);
    }
}
