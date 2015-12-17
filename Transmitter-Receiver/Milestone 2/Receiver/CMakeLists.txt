cmake_minimum_required(VERSION 2.8)
project(Receiver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Receiver.cpp physical/ReceivingUtils.cpp physical/ReceivingUtils.h physical/Hamming.h physical/Hamming.cpp enum/ErrorCodes.h enum/ErrorCorrection.h enum/OutputMode.h datalink/Frame.h datalink/Framing.cpp datalink/Framing.h physical/CRC.cpp physical/CRC.h)

add_executable(Receiver ${SOURCE_FILES})