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

ErrorCorrection error_correction;
int number_of_bits_to_clobber;
char *file_name;
char *port_number;
char *host_name;

int main(int argc, char *argv[])
{
    process_args(argc, argv);

    SendMode sendMode = SendMode::SOCKET;
    int sockfd;
    int number_of_bytes_per_frame = error_correction == ErrorCorrection::CRC ? 62 : 64;

    if (sendMode == SendMode::SOCKET) {
        connect_to_server(sockfd);
    }

    std::string message =
            retrieve_file_to_transmit(file_name);
    for (unsigned int i = 0; i < strlen(message.c_str()); i += number_of_bytes_per_frame) {
        Frame *frame = new Frame();
        build_frame(message.c_str(), i, frame, error_correction);
        send(frame, sendMode, sockfd, error_correction, number_of_bits_to_clobber);
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
    const char *args = "i:n:p:acb:h";
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
            case 'a':
                if (error_correction != ErrorCorrection::NONE) {
                    std::cerr << "You can only set one type of error detection/correction. Halting." << std::endl;
                    exit(ERROR_CODE_ONLY_ONE_ERROR_DETECTION_MODE_ALLOWED);
                }
                error_correction = ErrorCorrection::HAMMING;
                break;
            case 'c':
                if (error_correction != ErrorCorrection::NONE) {
                    std::cerr << "You can only set one type of error detection/correction. Halting." << std::endl;
                    exit(ERROR_CODE_ONLY_ONE_ERROR_DETECTION_MODE_ALLOWED);
                }
                error_correction = ErrorCorrection::CRC;
                break;
            case 'b':
                number_of_bits_to_clobber = atoi(optarg);
                break;
            case 'h':
                print_help();
                exit(0);
            case '?':
                std::cerr << "Unknown option: " << optopt << ". Exiting" << std::endl;
                exit(ERROR_UNKNOWN_PARAMETER);
            default:
                print_help();
                break;
        }
    }
}

void print_help() {
    std::cout << "Usage: Transmitter [-i <filename>] [-h <hostname>] [-p <port number>] [-a | -c] [-?]\n";
    std::cout << "-i: Input file. Can take a path relative to the executable's location, or an absolute path\n";
    std::cout << "-h: Hostname of the receiver. Can use IP address or DNS address.\n";
    std::cout << "-p: Port number the receiver is listening on.\n";
    std::cout << "-a: Error correction using Hamming encoding. This must be consistent with the receiver's error detection/correction mode\n";
    std::cout << "-c: Error detection using CRC. This must be consistent with the receiver's error detection/correction mode\n";
    std::cout << "*** Only -a or -c can be chosen, NOT both. ***\n";
    std::cout << "-?: Display this help text";
}