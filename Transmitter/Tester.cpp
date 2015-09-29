//
// Created by mkoch on 9/29/15.
//

// Random file used to test various aspects of the application
#include "physical/BinaryCalculator.h"
#include <iostream>
int main() {
    char *binary_value = new char[8];
    char *message = convert_char_to_binary(binary_value, 'a');

    for (int i = 0; i < 8; i++) {
        std::cout << (int)message[i];
    }
}