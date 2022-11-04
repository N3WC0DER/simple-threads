# Sorry, but I'm still not very strong in the CMake project build system.
# I try my best to make it convenient for other people to use my library!

cmake_minimum_required(VERSION 3.6.0)
project(simple-threads
        LANGUAGES CXX
        VERSION 0.1.1
        DESCRIPTION "Simple thread control")

get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

option(STH_BUILD_EXAMPLES "Build ${PROJECT_NAME} examples" OFF)
option(STH_GENERATE_DOCS "Generate ${PROJECT_NAME} docs" OFF)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

file(GLOB SRCS "${PROJECT_SOURCE_DIR}/src/*")

target_sources(${PROJECT_NAME} PRIVATE ${SRCS})

install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)

if(STH_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (STH_GENERATE_DOCS)
    add_subdirectory(doc)
endif ()