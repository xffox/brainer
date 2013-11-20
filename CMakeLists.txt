cmake_minimum_required(VERSION 2.6)

project(brainer)

if(CMAKE_BUILD_TYPE STREQUAL "debug")
    add_definitions(-DDEBUG)
endif()

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

if(UNIX)
    add_definitions("-Wall -Wextra -pedantic -Wno-long-long -std=c++11")
endif()

add_subdirectory(base)
add_subdirectory(engine)
add_subdirectory(core)
add_subdirectory(task)
add_subdirectory(gui)
