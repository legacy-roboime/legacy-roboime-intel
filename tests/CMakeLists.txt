cmake_minimum_required(VERSION 2.8)


##############################
# Test source files
#

set(test .)
set(INT_TEST_SRC
    ${test}/main.cc
    ${test}/sample1.cc
    ${test}/sample1_unittest.cc
)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
set(testapp roboime-intel-test)
add_executable(${testapp} ${INT_TEST_SRC})
target_link_libraries(${testapp} ${GTEST_BOTH_LIBRARIES})
#XXX note that the path to the test is not flexible, and should be
# properly updated if the bin dir changes
gtest_add_tests(${CMAKE_CURRENT_SOURCE_DIR}/../bin/${testapp} "" ${INT_TEST_SRC})
