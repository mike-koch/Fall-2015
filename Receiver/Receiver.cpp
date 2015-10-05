#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include "physical/TransmissionUtils.h"

void check_args(int argc, char *argv[]);
void connect_to_server(char *argv[], int &sockfd);
void error(char *msg) {
    perror(msg);
    exit(0);
}
bool socket_is_alive(int sockfd);

int main(int argc, char *argv[])
{
    int sockfd;

    check_args(argc, argv);
    connect_to_server(argv, sockfd);

    while (socket_is_alive(sockfd)) {
        read(sockfd);
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

void check_args(int argc, char *argv[]) {
    if( argc < 3 ) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
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