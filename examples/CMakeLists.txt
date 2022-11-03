cmake_minimum_required(VERSION 3.6.0)
project(example-simple-threads LANGUAGES CXX)
include("../cmake/utils.cmake")

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
        is_top_level)

if(is_top_level)
    find_package(simple-threads REQUIRED)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall")

set(sources main.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${sources})
target_link_libraries(${PROJECT_NAME} PRIVATE simple-threads::simple-threads)

if(NOT is_top_level)
    win_copy_deps_to_target_dir(${PROJECT_NAME} simple-threads::simple-threads)
endif()