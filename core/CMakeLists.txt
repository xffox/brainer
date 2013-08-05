cmake_minimum_required(VERSION 2.6)

set(CORE_NAME core)

set(CORE_INCLUDES "${CMAKE_SOURCE_DIR}/core/include")
set(CORE_INCLUDES ${CORE_INCLUDES} PARENT_SCOPE)

include_directories(${CORE_INCLUDES})

set(CORE_LIBS "")
