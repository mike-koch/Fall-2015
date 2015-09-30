//
// Created by mkoch on 9/29/15.
//

// Random file used to test various aspects of the application
#include "datalink/Framing.h"
#include "physical/TransmissionUtils.h"
#include "enum/SendMode.h"
#include <iostream>
int main() {
    char *my_string = "foo bar89012345678901234567890123";
    Frame *my_frame = new Frame();
    build_frame(my_string, 0, my_frame);
    std::cout << "Syn character: " << my_frame->first_syn << std::endl;
    std::cout << "Second syn character: " << my_frame->second_syn << std::endl;
    std::cout << "Length (int): " << (int)my_frame->length << std::endl;
    std::cout << "Length (char): " << my_frame->length << std::endl;
    std::cout << "Data: " << my_frame->data << std::endl;
    send(my_frame, SendMode::CONSOLE, -1);
}