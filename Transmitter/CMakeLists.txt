cmake_minimum_required(VERSION 2.8)
project(Transmitter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Transmitter.cpp app/FileManager.cpp app/FileManager.h error/ErrorCodes.h Tester.cpp)

add_executable(Transmitter ${SOURCE_FILES})