//
// Created by mkoch on 10/7/15.
//
#ifndef RECEIVER_FRAMING_H
#define RECEIVER_FRAMING_H

#include "Frame.h"
#include "../physical/ReceivingUtils.h"

// data: The UNPARSED data received from the Transmitter. This will not work if the data is already parsed!
//       data must also be no more than 64 characters long
// length: The PARSED integer representation of the length. Passing a char is permitted, as long as the int value of the
//         char is equivalent to the length
void build_frame(Frame *frame, int length, char *data);
#endif //RECEIVER_FRAMING_H
