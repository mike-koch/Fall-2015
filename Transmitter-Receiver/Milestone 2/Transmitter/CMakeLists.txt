cmake_minimum_required(VERSION 2.8)
project(Transmitter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Transmitter.cpp app/FileManager.cpp app/FileManager.h enum/ErrorCodes.h physical/TransmissionUtils.cpp physical/TransmissionUtils.h datalink/Frame.h datalink/Framing.cpp datalink/Framing.h enum/SendMode.h physical/CRC.cpp physical/CRC.h physical/Hamming.cpp physical/Hamming.h enum/ErrorCorrection.h)

add_executable(Transmitter ${SOURCE_FILES})
