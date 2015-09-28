cmake_minimum_required(VERSION 3.3)
project(Transmitter_Receiver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Transmitter.cpp Receiver.cpp)

add_executable(Transmitter_Receiver ${SOURCE_FILES})