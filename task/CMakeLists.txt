cmake_minimum_required(VERSION 3.0)

set(TASK_LIB task)
set(TASK_LIB ${TASK_LIB} PARENT_SCOPE)

set(TASK_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CSV_INCLUDES}")

file(GLOB TASK_SRCS "src/*.cpp" "src/*/*.cpp")

add_library(${TASK_LIB} ${TASK_SRCS})
target_include_directories(${TASK_LIB} PUBLIC "${TASK_INCLUDES}")
target_link_libraries(${TASK_LIB} ${CORE_LIB} ${MASTERMIND_LIB})

add_subdirectory(test)
