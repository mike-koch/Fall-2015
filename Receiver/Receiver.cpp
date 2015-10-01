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
void connect_to_server(int &portno, char *argv[], int &sockfd, hostent *server, sockaddr_in &serv_addr);
void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    check_args(argc, argv);
    connect_to_server(portno, argv, sockfd, server, serv_addr);

    read(sockfd);

    return 0;
}

void check_args(int argc, char *argv[]) {
    if( argc < 3 ) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
}

void connect_to_server(int &portno, char *argv[], int &sockfd, hostent *server, sockaddr_in &serv_addr) {
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if( sockfd < 0 )
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
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
    serv_addr.sin_port = htons(portno);
    if( connect(sockfd, (const sockaddr *) &serv_addr,sizeof(serv_addr)) < 0 )
        error("ERROR connecting");
}