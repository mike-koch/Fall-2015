//
// Created by mkoch on 10/8/15.
//

#ifndef RECEIVER_FILEMANAGER_H
#define RECEIVER_FILEMANAGER_H

#include <iosfwd>
#include <fstream>

std::ofstream open_output_file(const char *file_path);
void write_to_file(std::ofstream file_stream, const char *contents, int size);
#endif //RECEIVER_FILEMANAGER_H
