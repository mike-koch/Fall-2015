#include "Framing.h"
int get_end_offset(const char *data, unsigned int offset);

// Takes a char array of the data to be framed, starting at the current offset. Returns the same frame that is passed
//    in, but populated. Offset is 0-indexed.
Frame* build_frame(const char *data, unsigned int offset, Frame *frame_to_populate) {

    // We get the end offset through a helper function to prevent accidentally going beyond the end of the string.
    unsigned int end_offset = get_end_offset(data, offset);
    unsigned int length = end_offset - offset;

    frame_to_populate->length = length;

    // Just place each byte of data into the frame. Nothing fancy.
    for (int i = 0; i < length; i++) {
        frame_to_populate->data[i] = data[offset + i];
    }

    return frame_to_populate;
}

// Helper function to get the "actual" end index, in case the next frame is going to be less than 64 bytes. Offset is 0-indexed
int get_end_offset(const char *data, unsigned int offset) {
    const int offset_amount = 64;
    unsigned int length_to_process = offset + offset_amount;
    size_t length_of_data = strlen(data);

    if (length_to_process > length_of_data) {
        return length_of_data;
    }

    return length_to_process;
}