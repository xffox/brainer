cmake_minimum_required(VERSION 3.0)

set(BRAINER_TASK_LIB brainertask)
set(BRAINER_TASK_LIB ${BRAINER_TASK_LIB} PARENT_SCOPE)

set(BRAINER_TASK_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB BRAINER_TASK_SRCS "src/*.cpp" "src/*/*.cpp")

add_library(${BRAINER_TASK_LIB} ${BRAINER_TASK_SRCS})
target_include_directories(${BRAINER_TASK_LIB} PUBLIC "${BRAINER_TASK_INCLUDES}")
target_link_libraries(${BRAINER_TASK_LIB} ${BRAINER_CORE_LIB} ${BRAINER_CSV_LIB}
    ${BRAINER_MASTERMIND_LIB})

add_subdirectory(test)
