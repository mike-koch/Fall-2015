//
// Created by mkoch on 10/8/15.
//

#include "FileManager.h"
std::ofstream open_output_file(const char *file_path) {
    std::ofstream output_stream;
    output_stream.open(file_path);
    return output_stream;
}

void write_to_file(std::ofstream file_stream, const char *contents, int size) {
    file_stream.write(contents, size);
    return;
}