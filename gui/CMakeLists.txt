cmake_minimum_required(VERSION 3.0)

set(GUI_NAME brainer_gui)

message(STATUS "gui is enabled")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(GUI_SRCS
    "src/main.cpp"
    "src/Application.cpp"
    "src/MenuDialog.cpp"
    "src/TaskDialog.cpp"
    )

add_executable(${GUI_NAME} ${GUI_SRCS})
target_link_libraries(${GUI_NAME} Qt5::Widgets
    ${BRAINER_CORE_LIB} ${BRAINER_TASK_LIB} ${BRAINER_BASE_LIB})
install(TARGETS ${GUI_NAME} RUNTIME DESTINATION "bin")
