cmake_minimum_required(VERSION 3.0)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/test")

include(FindPkgConfig)
pkg_search_module(CPPUNIT cppunit)
if(CPPUNIT_FOUND)
    set(BRAINER_CSV_TEST brainer_csv_test)

    file(GLOB BRAINER_CSV_TEST_SRCS "*.cpp")

    add_executable(${BRAINER_CSV_TEST} ${BRAINER_CSV_TEST_SRCS})
    target_link_libraries(${BRAINER_CSV_TEST}
        ${CPPUNIT_LIBRARIES} ${BRAINER_CSV_LIB})

    add_test(NAME CsvTest COMMAND ${BRAINER_CSV_TEST})
else(CPPUNIT_FOUND)
    message(WARNING "cppunit not found - testing is not available")
endif(CPPUNIT_FOUND)
