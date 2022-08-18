cmake_minimum_required(VERSION 3.0)

set(CORE_LIB core)
set(CORE_LIB ${CORE_LIB} PARENT_SCOPE)

set(CORE_INCLUDES "${CMAKE_SOURCE_DIR}/core/include")

set(CORE_SRCS
    "src/TaskLogic.cpp"
    "src/Stopwatch.cpp"
    )

add_library(${CORE_LIB} ${CORE_SRCS})
target_include_directories(${CORE_LIB} PUBLIC "${CORE_INCLUDES}")
target_link_libraries(${CORE_LIB} ${BASE_LIB})
