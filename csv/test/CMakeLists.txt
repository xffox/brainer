cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/test")

include(FindPkgConfig)
pkg_search_module(CPPUNIT cppunit)
if(CPPUNIT_FOUND)
    set(CSV_TEST_NAME csv_test)

    file(GLOB CSV_TEST_SRCS "*.cpp")

    add_executable(${CSV_TEST_NAME} ${CSV_TEST_SRCS})
    target_link_libraries(${CSV_TEST_NAME}
        ${CPPUNIT_LIBRARIES})

    add_test(NAME CsvTest COMMAND ${CSV_TEST_NAME})
else(CPPUNIT_FOUND)
    message(WARNING "cppunit not found - testing is not available")
endif(CPPUNIT_FOUND)
