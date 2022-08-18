cmake_minimum_required(VERSION 3.0)

set(BASE_LIB "base")
set(BASE_LIB ${BASE_LIB} PARENT_SCOPE)

set(BASE_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB BASE_SRCS "src/*.cpp")

add_library(${BASE_LIB} ${BASE_SRCS})
target_include_directories(${BASE_LIB} PUBLIC "${BASE_INCLUDES}")
