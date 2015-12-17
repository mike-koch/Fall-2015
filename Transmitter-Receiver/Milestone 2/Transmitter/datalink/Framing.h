#ifndef TRANSMITTER_FRAMING_H
#define TRANSMITTER_FRAMING_H

#include "Frame.h"
#include "../enum/ErrorCorrection.h"

Frame* build_frame(const char *data, unsigned int offset, Frame *frame_to_populate, ErrorCorrection error_correction_mode);
#endif //TRANSMITTER_FRAMING_H
