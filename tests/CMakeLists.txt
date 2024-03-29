cmake_minimum_required(VERSION 2.8)

set(testapp roboime-intel-test)
set(test .)

##############################
# Test source files
#

set(INT_TEST_SRC
    ${test}/main.cc
    ${test}/robot_unittest.cc
    ${test}/actuator_unittest.cc
    ${test}/ball_unittest.cc
    ${test}/battery_unittest.cc
    ${test}/component_unittest.cc
    ${test}/body_unittest.cc
    ${test}/dribbler_unittest.cc
    ${test}/goal_unittest.cc
    ${test}/kicker_unittest.cc
    ${test}/wheel_unittest.cc
    ${test}/object_unittest.cc
)

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${INT_INC_DIRS})
include_directories(${PROTOBUF_INCLUDE_DIRS})

add_executable(${testapp} ${INT_TEST_SRC})
target_link_libraries(${testapp} ${GTEST_BOTH_LIBRARIES} roboime-intel)
qt5_use_modules(${testapp} Core Network Widgets)

#XXX note that the path to the test is not flexible, and should be
# properly updated if the bin dir changes
gtest_add_tests(${PROJECT_SOURCE_DIR}/bin/${testapp} "" ${INT_TEST_SRC})
