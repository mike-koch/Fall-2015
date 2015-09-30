//
// Created by mkoch on 9/29/15.
//

#ifndef TRANSMITTER_FRAME_H
#define TRANSMITTER_FRAME_H

#include <string.h>

static char SYN = 23;
struct Frame {

    char first_syn = SYN;
    char second_syn = SYN;
    char data[64];
    char length;
};
#endif //TRANSMITTER_FRAME_H
