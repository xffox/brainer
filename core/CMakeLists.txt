cmake_minimum_required(VERSION 2.6)

set(CORE_NAME core)

set(CORE_INCLUDES "${CMAKE_SOURCE_DIR}/core/include"
    "${CMAKE_SOURCE_DIR}/base/include")
set(CORE_INCLUDES ${CORE_INCLUDES} PARENT_SCOPE)

include_directories(${CORE_INCLUDES})

set(CORE_LIBS ${CORE_NAME})
set(CORE_LIBS ${CORE_LIBS} PARENT_SCOPE)

set(CORE_SRCS
    "src/TaskLogic.cpp"
    "src/Stopwatch.cpp"
    "src/FileDictProvider.cpp"
    )

add_library(${CORE_NAME} ${CORE_SRCS})
