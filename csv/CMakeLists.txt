cmake_minimum_required(VERSION 3.0)

set(BRAINER_CSV_LIB "brainercsv")
set(BRAINER_CSV_LIB ${BRAINER_CSV_LIB} PARENT_SCOPE)

set(BRAINER_CSV_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(${BRAINER_CSV_LIB} INTERFACE)
target_include_directories(${BRAINER_CSV_LIB} INTERFACE "${BRAINER_CSV_INCLUDES}")

add_subdirectory("test")
