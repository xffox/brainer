cmake_minimum_required(VERSION 3.0)

project(brainer)

include(CTest)

set(CPACK_GENERATOR "TGZ")
include(CPack)

if(CMAKE_BUILD_TYPE STREQUAL "debug")
    add_definitions(-DDEBUG)
endif()

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

if(UNIX)
    add_definitions("-Wall -Wextra -Weffc++ -pedantic -std=c++17")
endif()

add_subdirectory(base)
add_subdirectory(xlog/xlog)
add_subdirectory(csv)
add_subdirectory(core)
add_subdirectory(task)
if(ENABLE_GUI)
    add_subdirectory(gui)
endif()
if(ENABLE_BOT)
    add_subdirectory(bot)
endif()
