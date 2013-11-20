cmake_minimum_required(VERSION 2.6)

find_package(Qt4)
if(NOT QT4_FOUND)
    message(FATAL_ERROR "Qt not found")
endif()

set(GUI_NAME brainer)

include(${QT_USE_FILE})

include_directories(
    ${BASE_INCLUDES}
    "${CMAKE_CURRENT_BINARY_DIR}" # ui headers
    ${CORE_INCLUDES}
    ${TASK_INCLUDES}
    )

set(GUI_SRCS
    "src/main.cpp"
    "src/MenuDialog.cpp"
    "src/TaskDialog.cpp"
    )

set(GUI_MOC_HDRS
    "src/MenuDialog.h"
    "src/TaskDialog.h"
    )

set(GUI_UIS
    "ui/MenuDialog.ui"
    "ui/TaskDialog.ui"
    )

qt4_wrap_cpp(GUI_MOC_SRCS ${GUI_MOC_HDRS})

qt4_wrap_ui(GUI_UI_HDRS ${GUI_UIS})

#qt4_add_resources(GUI_RES_SRCS ${GUI_RES})

add_executable(${GUI_NAME} ${GUI_SRCS} ${GUI_MOC_SRCS} ${GUI_UI_HDRS})
target_link_libraries(${GUI_NAME} ${QT_QTMAIN_LIBRARY} ${QT_LIBRARIES}
    ${CORE_LIBS} ${TASK_LIBS} ${BASE_LIBS})
