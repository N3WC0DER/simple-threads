cmake_minimum_required(VERSION 3.6.0)
project(simple-threads-tests)

include(../cmake/utils.cmake)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
    is_top_level)

if (is_top_level)
    enable_testing()
    find_package(simple-threads REQUIRED)
endif ()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE gtest_main simple-threads::simple-threads)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
