cmake_minimum_required(VERSION 2.8)

##############################
# Include all headers
#

include_directories(. robot interface)
file(GLOB_RECURSE SIM_HDR "*.h")

##############################
# Files which will be compiled
#

set(SIM_SRC
    interface/UDPMulticastSenderSSLVision.cpp
    interface/UDPServerSimInt.cpp
    # add new sources here

    robot/NxAllRobots.cpp
    robot/NxDribbler.cpp
    robot/NxRobot.cpp
    robot/NxKicker.cpp
    robot/NxVehicle.cpp
    robot/NxWheel.cpp
    # add new sources here

    Simulation.cpp
    MyContactReport.cpp
    MyUserNotify.cpp
    NxAllBalls.cpp
    NxAllFields.cpp
    NxScene1.cpp
    NxBall.cpp
    NxMath1.cpp
    NxField.cpp
    # add new sources here
)



##############################
# Intra dependencies
#

# SamplesCommonCode
include_directories(${SCC_INC_DIRS})

# NxuStream2
include_directories(${NXU_INC_DIRS})

# Communication
include_directories(${COMM_INC_DIRS})



##############################
# Le finale
#

# adding to group
directory_source_group(headers . *.h)
directory_source_group(src . *.cpp)
source_group(generated REGULAR_EXPRESSION ".*_automoc[.]cpp")

set(SIM_INC_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/interface
    ${CMAKE_CURRENT_SOURCE_DIR}/robot
    ${CMAKE_CURRENT_BINARY_DIR}
    ${COMM_INC_DIRS}
    ${NXU_INC_DIRS}
    ${SCC_INC_DIRS}
    ${PHYSX_INCLUDES}
    PARENT_SCOPE
)

add_library(simulation
    ${SIM_HDR}
    ${SIM_SRC}
)

target_link_libraries(simulation
    ${PHYSX_LIBRARIES}
    ${QT_LIBRARIES}
    samplescommoncode
    nxustream2
    communication
)

set_property(TARGET simulation PROPERTY FOLDER modules)
