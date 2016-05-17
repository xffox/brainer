cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/test")

include(FindPkgConfig)
pkg_search_module(CPPUNIT cppunit)
if(CPPUNIT_FOUND)
    set(FACT_TEST_NAME fact_test)

    file(GLOB FACT_TEST_SRCS "*.cpp")

    add_executable(${FACT_TEST_NAME} ${FACT_TEST_SRCS})
    target_link_libraries(${FACT_TEST_NAME} ${FACT_LIBS}
        ${CPPUNIT_LIBRARIES})

    add_test(NAME FactTest COMMAND ${FACT_TEST_NAME})
else(CPPUNIT_FOUND)
    message(WARNING "cppunit not found - testing is not available")
endif(CPPUNIT_FOUND)
