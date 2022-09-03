cmake_minimum_required(VERSION 3.0)

set(BRAINER_CORE_LIB brainercore)
set(BRAINER_CORE_LIB ${BRAINER_CORE_LIB} PARENT_SCOPE)

set(BRAINER_CORE_INCLUDES "${CMAKE_SOURCE_DIR}/core/include")

set(BRAINER_CORE_SRCS
    "src/TaskLogic.cpp"
    "src/Stopwatch.cpp"
    )

add_library(${BRAINER_CORE_LIB} ${BRAINER_CORE_SRCS})
target_include_directories(${BRAINER_CORE_LIB} PUBLIC "${BRAINER_CORE_INCLUDES}")
target_link_libraries(${BRAINER_CORE_LIB} ${BRAINER_BASE_LIB})
