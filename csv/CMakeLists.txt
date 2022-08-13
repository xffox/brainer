cmake_minimum_required(VERSION 3.0)

set(CSV_NAME "csv")

set(CSV_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(CSV_INCLUDES ${CSV_INCLUDES} PARENT_SCOPE)

include_directories(${CSV_INCLUDES})

add_subdirectory("test")
