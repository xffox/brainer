cmake_minimum_required(VERSION 2.6)

set(TEST_WIKTIONARY_NAME "test_wiktionary")

include_directories(${CSV_INCLUDES})

add_executable(${TEST_WIKTIONARY_NAME} "testwiktionary.cpp")
target_link_libraries(${TEST_WIKTIONARY_NAME} ${TASK_LIB})
