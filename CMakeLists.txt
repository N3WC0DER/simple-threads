# Sorry, but I'm still not very strong in the CMake project build system.
# I try my best to make it convenient for other people to use my library!

cmake_minimum_required(VERSION 3.6.0)
project(simple-threads
        LANGUAGES CXX
        VERSION 0.1.1
        DESCRIPTION "Simple thread control")

add_library(simple-threads)
add_library(simple-threads::simple-threads ALIAS simple-threads)

option(STH_BUILD_EXAMPLES "Build simple-threads examples" OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall")

target_include_directories(simple-threads
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

file(GLOB SRCS "${PROJECT_SOURCE_DIR}/src/*")

target_sources(${PROJECT_NAME} PRIVATE ${SRCS})

if(STH_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()