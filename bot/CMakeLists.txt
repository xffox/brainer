cmake_minimum_required(VERSION 2.6)

set(BOT_NAME brainer_bot)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLOOX REQUIRED gloox)

set(BOT_INCLUDES ${CORE_INCLUDES} ${TASK_INCLUDES} ${GLOOX_INCLUDE_DIRS})
set(BOT_LIBS ${CORE_LIBS} ${TASK_LIBS} ${GLOOX_LIBRARIES})

include_directories(${BOT_INCLUDES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GLOOX_CFLAGS}")

file(GLOB BOT_SRCS "src/*.cpp")

add_executable(${BOT_NAME} ${BOT_SRCS})
target_link_libraries(${BOT_NAME} ${BOT_LIBS})
