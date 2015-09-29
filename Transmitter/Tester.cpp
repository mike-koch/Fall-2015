//
// Created by mkoch on 9/29/15.
//

// Random file used to test various aspects of the application
#include "app/FileManager.h"
#include <iostream>
int main() {
    const char* message =
            retrieve_file_to_transmit("/home/mkoch/ClionProjects/Transmitter-Receiver/Transmitter/sample-input.txt");

    std::cout << message << std::endl;
}