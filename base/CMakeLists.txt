cmake_minimum_required(VERSION 3.0)

set(BASE_NAME "base")

set(BASE_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(BASE_INCLUDES ${BASE_INCLUDES} PARENT_SCOPE)

set(BASE_LIBS  ${BASE_NAME})
set(BASE_LIBS ${BASE_LIBS} PARENT_SCOPE)

include_directories(${BASE_INCLUDES})

file(GLOB BASE_SRCS "src/*.cpp")

add_library(${BASE_NAME} ${BASE_SRCS})
