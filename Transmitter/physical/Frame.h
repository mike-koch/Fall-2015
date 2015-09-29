//
// Created by mkoch on 9/29/15.
//

#ifndef TRANSMITTER_FRAME_H
#define TRANSMITTER_FRAME_H
struct Frame {
    char first_syn = 22;
    char second_syn = 22;
    int length;
    char* data;
};
#endif //TRANSMITTER_FRAME_H
