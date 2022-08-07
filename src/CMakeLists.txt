# Sorry, but I'm still not very strong in the CMake project build system.
# I try my best to make it convenient for other people to use my library!

cmake_minimum_required(VERSION 3.6.0)
project(simple-threads)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall")

include_directories(${PROJECT_SOURCE_DIR}/)

file(GLOB SRCS "${PROJECT_SOURCE_DIR}/*")

add_library(${PROJECT_NAME} STATIC ${SRCS})
