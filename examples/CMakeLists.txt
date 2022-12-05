cmake_minimum_required(VERSION 3.25.0)
project(simple-threads-examples LANGUAGES CXX C)
include("../cmake/utils.cmake")

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
        is_top_level)

if(is_top_level)
    find_package(simple-threads REQUIRED)
endif()

file(GLOB_RECURSE SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SRCS})
target_link_libraries(${PROJECT_NAME} PRIVATE simple-threads::simple-threads)
