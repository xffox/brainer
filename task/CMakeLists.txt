cmake_minimum_required(VERSION 2.6)

set(TASK_NAME task)

set(TASK_INCLUDES 
    "${CMAKE_SOURCE_DIR}/task/include"
    ${CORE_INCLUDES}
    )
set(TASK_INCLUDES ${TASK_INCLUDES} PARENT_SCOPE)

set(TASK_LIBS ${TASK_NAME})
set(TASK_LIBS ${TASK_LIBS} PARENT_SCOPE)

include_directories(${TASK_INCLUDES})

set(TASK_SRCS
    "src/HexByteTask.cpp"
    "src/HexByteTaskGenerator.cpp"
    "src/MultiplicationTask.cpp"
    "src/MultiplicationTaskGenerator.cpp"
    "src/DictTask.cpp"
    "src/DictTaskGenerator.cpp"
    "src/TaskProvider.cpp"
    )

add_library(${TASK_NAME} ${TASK_SRCS})
target_link_libraries(${TASK_NAME} ${CORE_LIBS})
