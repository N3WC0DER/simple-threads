cmake_minimum_required(VERSION 3.6.0)
project(example-simple-threads)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall")

include_directories(
  ${PROJECT_SOURCE_DIR}/
)

file(GLOB SRCS
  "${PROJECT_SOURCE_DIR}/*"
)

# Connecting the library, specify where to get the header files
include_directories("${PROJECT_SOURCE_DIR}/../bin/include/")

find_library(SIMPLE_THREADS simple-threads ${PROJECT_SOURCE_DIR}/../bin/)

add_executable(${PROJECT_NAME} ${SRCS})

# And also we specify dependence on static library
target_link_libraries(${PROJECT_NAME} PRIVATE ${SIMPLE_THREADS})