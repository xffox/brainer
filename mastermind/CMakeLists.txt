cmake_minimum_required(VERSION 3.5)

set(MASTERMIND_LIB "mastermind")
set(MASTERMIND_LIB ${MASTERMIND_LIB} PARENT_SCOPE)

file(GLOB MASTERMIND_SRCS "src/*.cpp")

add_library(${MASTERMIND_LIB} ${MASTERMIND_SRCS})
target_include_directories(${MASTERMIND_LIB}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
