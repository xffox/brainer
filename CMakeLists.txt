cmake_minimum_required(VERSION 3.0)

project(brainer)

include(CTest)

set(CPACK_GENERATOR "TGZ")
include(CPack)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

if(UNIX)
    add_definitions("-Wall -Wextra -Weffc++ -pedantic -Werror")
endif()

add_subdirectory(base)
add_subdirectory(csv)
add_subdirectory(core)
add_subdirectory(mastermind)
add_subdirectory(task)
if(ENABLE_GUI)
    add_subdirectory(gui)
endif()
if(ENABLE_BOT)
    add_subdirectory(bot)
endif()
