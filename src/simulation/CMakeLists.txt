cmake_minimum_required(VERSION 2.8)

##############################
# Files which need to be moc'd
# Ideally this should be automatic, but it isn't yet
#

set(SIM_MOC
    Simulation.h
    interface/UDPMulticastSenderSSLVision.h
    interface/UDPServerSimInt.h
    # add new headers here
)

##############################
# Include all headers
#

include_directories(. robot interface)

file(GLOB SIM_HDR "interface/*.h")
source_group("Headers\\Interface" FILES ${SIM_HDR})

file(GLOB SIM_HDR "robot/*.h")
source_group("Headers\\Robot" FILES ${SIM_HDR})

file(GLOB SIM_HDR "*.h")
source_group("Headers" FILES ${SIM_HDR})

file(GLOB_RECURSE SIM_HDR "*.h")

##############################
# Files which will be compiled
#

set(SIM_SRC_INTERFACE
    interface/UDPMulticastSenderSSLVision.cpp
    interface/UDPServerSimInt.cpp
    # add new sources here
)
source_group("Sources\\Interface" FILES ${SIM_SRC_INTERFACE})

set(SIM_SRC_ROBOT
    robot/NxAllRobots.cpp
    robot/NxDribbler.cpp
    robot/NxRobot.cpp
    robot/NxKicker.cpp
    robot/NxVehicle.cpp
    robot/NxWheel.cpp
    # add new sources here
)
source_group("Sources\\Robot" FILES ${SIM_SRC_ROBOT})

set(SIM_SRC_OTHERS
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
source_group("Sources" FILES ${SIM_SRC_OTHERS})

set(SIM_SRC
    ${SIM_SRC_INTERFACE}
    ${SIM_SRC_ROBOT}
    ${SIM_SRC_OTHERS}
    # add new source groups here
)



##############################
# Intra dependencies
#

# SamplesCommonCode
add_subdirectory(samplescommoncode)
include_directories(${SCC_INC_DIRS})

# NxuStream2
add_subdirectory(nxustream2)
include_directories(${NXU_INC_DIRS})

# Communication
include_directories(${COMM_INC_DIRS})



##############################
# Qt stuff
#

find_package(Qt4 REQUIRED)
set(QT_DONT_USE_QTGUI true)
set(QT_USE_QTNETWORK true)
include(${QT_USE_FILE})
qt4_wrap_cpp(SIM_MOC_CPP ${SIM_MOC})



##############################
# PhysX stuff
#

find_package(PhysX REQUIRED)
include_directories(${PHYSX_INCLUDES})



##############################
# Le finale
#

set(GEN_FILES
    ${SIM_MOC_CPP}
)

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

# adding to group
source_group("Generated" FILES ${GEN_FILES})

add_library(simulation
    ${GEN_FILES}
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
