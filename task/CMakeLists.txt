cmake_minimum_required(VERSION 2.6)

set(TASK_NAME task)

set(TASK_INCLUDES 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    ${CORE_INCLUDES}
    ${CSV_INCLUDES}
    )
set(TASK_INCLUDES ${TASK_INCLUDES} PARENT_SCOPE)

set(TASK_LIBS ${TASK_NAME} ${CORE_LIBS})
set(TASK_LIBS ${TASK_LIBS} PARENT_SCOPE)

include_directories(${TASK_INCLUDES})

file(GLOB TASK_SRCS "src/*.cpp")

add_library(${TASK_NAME} ${TASK_SRCS})
