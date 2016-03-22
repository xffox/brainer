cmake_minimum_required(VERSION 2.6)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/test")

set(ENGINE_TEST_NAME msgtest)
set(ENGINE_TEST_SRCS
    "main.cpp"
    )

include_directories(${ENGINE_INCLUDES})

add_executable(${ENGINE_TEST_NAME} ${ENGINE_TEST_SRCS})
target_link_libraries(${ENGINE_TEST_NAME} ${ENGINE_LIBS})
