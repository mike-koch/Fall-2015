#ifndef TRANSMITTER_FRAME_H
#define TRANSMITTER_FRAME_H

#include <string>
#include <bits/stringfwd.h>

const char SYN = 22;
struct Frame {

    char first_syn = SYN;
    char second_syn = SYN;
    char length;
    char data[64];
};
#endif //TRANSMITTER_FRAME_H
