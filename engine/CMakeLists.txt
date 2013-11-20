cmake_minimum_required(VERSION 2.6)

set(ENGINE_NAME engine)

set(ENGINE_INCLUDES "${CMAKE_SOURCE_DIR}/engine/include")
set(ENGINE_INCLUDES ${ENGINE_INCLUDES} PARENT_SCOPE)

include_directories(${ENGINE_INCLUDES})

#set(ENGINE_LIBS ${ENGINE_NAME})
#set(ENGINE_LIBS ${ENGINE_LIBS} PARENT_SCOPE)

add_subdirectory(test)
