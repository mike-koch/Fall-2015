#ifndef RECEIVER_ERRORCODES_H
#define RECEIVER_ERRORCODES_H
//
// Created by mkoch on 9/29/15.
//
enum ErrorCode {
    ERROR_MALFORMED_CONTENT = 250,
    ERROR_UNABLE_TO_READ_FROM_SOCKET = 350,
    ERROR_UNKNOWN_PARAMETER = 450,
};

#endif //RECEIVER_ERRORCODES_H