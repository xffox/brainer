cmake_minimum_required(VERSION 2.6)

set(FACT_NAME fact)

set(FACT_INCLUDES 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    ${BASE_INCLUDES}
    ${CSV_INCLUDES}
    ${VARIANT_INCLUDES}
    )
set(FACT_INCLUDES ${FACT_INCLUDES} PARENT_SCOPE)

set(FACT_LIBS ${FACT_NAME} ${BASE_LIBS})
set(FACT_LIBS ${FACT_LIBS} PARENT_SCOPE)

include_directories(${FACT_INCLUDES})

file(GLOB FACT_SRCS "src/*.cpp")

add_library(${FACT_NAME} ${FACT_SRCS})

add_subdirectory(test)
