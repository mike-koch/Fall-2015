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
#include "enum/ErrorCodes.h"

using namespace std;

void error(const char* msg);
void process_args(int argc, char *argv[]);
void connect_to_server(int &sockfd);
void print_help();

char *file_name;
char *port_number;
char *host_name;

int main(int argc, char *argv[])
{
    int sockfd;

    process_args(argc, argv);

    SendMode sendMode = SendMode::SOCKET;

    if (sendMode == SendMode::SOCKET) {
        connect_to_server(sockfd);
    }

    std::string message =
            retrieve_file_to_transmit(argv[3]);
    for (unsigned int i = 0; i < strlen(message.c_str()); i += 64) {
        Frame *frame = new Frame();
        build_frame(message.c_str(), i, frame);
        send(frame, sendMode, sockfd);
        delete frame;
    }

    close(sockfd);
    
    return 0;
}

void connect_to_server(int &sockfd) {
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if( sockfd < 0 )
        error("ERROR opening socket");
    hostent *server = gethostbyname(host_name);
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
    serv_addr.sin_port = htons(atoi(port_number));
    if( connect(sockfd, (const sockaddr *) &serv_addr,sizeof(serv_addr)) < 0 )
        error("ERROR connecting");
}

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void process_args(int argc, char *argv[]) {
    const char *args = "i:n:p:h";
    int c;

    while ((c = getopt(argc, argv, args)) != -1) {
        switch (c) {
            case 'i':
                file_name = optarg;
                break;
            case 'n':
                host_name = optarg;
                break;
            case 'p':
                port_number = optarg;
                break;
            case '?':
                std::cerr << "Unknown option: " << optopt << ". Exiting" << std::endl;
                exit(ERROR_UNKNOWN_PARAMETER);
            case 'h':
            default:
                print_help();
                exit(0);
                break;
        }
    }
}

void print_help() {
    std::cout << "Usage: Transmitter [-i <filename>] [-n <hostname>] [-p <port number>] [-h]\n";
    std::cout << "-i: Input file. Can take a path relative to the executable's location, or an absolute path\n";
    std::cout << "-n: Hostname of the receiver. Can use IP address or DNS address.\n";
    std::cout << "-p: Port number the receiver is listening on.\n";
    std::cout << "-h: Display this help text";
}