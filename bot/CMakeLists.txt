cmake_minimum_required(VERSION 3.0)

set(BRAINER_BOT brainer_bot)
set(BRAINER_BOT_LIB brainer_bot_lib)

message(STATUS "bot is enabled")

find_package(PkgConfig REQUIRED)
find_package(XLog REQUIRED)
pkg_check_modules(GLOOX REQUIRED gloox)

set(BOT_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/src" "${GLOOX_INCLUDE_DIRS}")
set(BOT_LIBS ${CORE_LIB} ${TASK_LIB} ${GLOOX_LIBRARIES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GLOOX_CFLAGS_OTHER}")

file(GLOB BOT_SRCS "src/*.cpp")
list(REMOVE_ITEM "src/main.cpp")

add_library(${BRAINER_BOT_LIB} ${BOT_SRCS})
target_include_directories(${BRAINER_BOT_LIB} PUBLIC "${BOT_INCLUDES}")
target_link_libraries(${BRAINER_BOT_LIB} PUBLIC "${BOT_LIBS}" xlog::xlog)

add_executable(${BRAINER_BOT} "src/main.cpp")
target_link_libraries(${BRAINER_BOT} ${BRAINER_BOT_LIB})

install(TARGETS ${BRAINER_BOT} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
