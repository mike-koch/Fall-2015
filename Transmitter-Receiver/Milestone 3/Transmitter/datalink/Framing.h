#ifndef TRANSMITTER_FRAMING_H
#define TRANSMITTER_FRAMING_H

#include "Frame.h"

Frame* build_frame(const char *data, unsigned int offset, Frame *frame_to_populate);
#endif //TRANSMITTER_FRAMING_H
