#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "../enum/ErrorCodes.h"
std::string retrieve_file_to_transmit(char *file_path) {
    // This fancy simple code obtained via http://stackoverflow.com/a/2602258/1509431. This is much easier than
    //    reading each char at a time. Essentially we open an input stream to the file, and then read the entire
    //    file contents into a buffer, and then return the buffer as a string.
    std::ifstream input_stream(file_path);
    if (input_stream.fail()) {
        exit(ERROR_UNABLE_TO_OPEN_FILE);
    }

    std::stringstream buffer;
    buffer << input_stream.rdbuf();
    return buffer.str();
}
