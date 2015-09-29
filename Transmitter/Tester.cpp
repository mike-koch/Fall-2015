//
// Created by mkoch on 9/29/15.
//

// Random file used to test various aspects of the application
#include "physical/TransmissionUtils.h"
#include <iostream>
int main() {
    char *binary_value = new char[8];
    char *message = convert_char_to_binary(binary_value, 'c');
    int parity_bit = calculate_parity_bit(message);

    for (int i = 0; i < 8; i++) {
        std::cout << (int)message[i];
    }
    std::cout << std::endl << parity_bit << std::endl;
}