cmake_minimum_required(VERSION 2.8.9)

project(roboime-intel)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
include(${PROJECT_SOURCE_DIR}/cmake/Utils.cmake)

standard_config()
standard_paths(${PROJECT_SOURCE_DIR} bin lib)
#message(STATUS "Project src dir: ${PROJECT_SOURCE_DIR}")

# Qt stuff
find_package(Qt5)
find_package(Qt5Core)

#add_definitions(-DUSE_PHYSX)
#add_definitions(-DUSE_SIMULATION)

# Protobuf stuff
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

# Set where the source is contained.
set(src ./src)

##############################
# Include all headers
#

set(INT_INC
    ${src}
    ${src}/base
    ${src}/core
    ${src}/core/filters
    ${src}/core/plays
    ${src}/core/skills
    ${src}/core/tactics
    ${src}/interface
    ${src}/interface/base
    ${src}/interface/commanders
    ${src}/interface/updaters
    ${src}/prototypes
    ${src}/utils
    ${src}/utils/tree utils/kalman
)
include_directories(${INT_INC})

file(GLOB_RECURSE INT_HDR RELATIVE ${src} "*.h")

##############################
# Files which will be compiled
#

set(INT_SRC
    ${src}/base/Actuator.cpp
    ${src}/base/Team.cpp
    ${src}/base/Ball.cpp
    ${src}/base/StateMachine.cpp
    ${src}/base/Battery.cpp
    ${src}/base/State.cpp
    ${src}/base/Body.cpp
    ${src}/base/StageY.cpp
    ${src}/base/Component.cpp
    ${src}/base/Dribbler.cpp
    ${src}/base/Stage.cpp
    ${src}/base/Goal.cpp
    ${src}/base/Robot.cpp
    ${src}/base/Kicker.cpp
    ${src}/base/Object.cpp
    ${src}/base/MachineTransition.cpp
    ${src}/base/Wheel.cpp
    # add new sources here

    ${src}/core/filters/KalmanFilters.cpp
    ${src}/core/filters/KalmanFilter.cpp
    ${src}/core/filters/Filter.cpp
    # add new sources here

    ${src}/core/plays/Play.cpp
    ${src}/core/plays/CBR2011.cpp
    ${src}/core/plays/FreeKickThem.cpp
    ${src}/core/plays/Halt.cpp
    ${src}/core/plays/LinearCombPlay.cpp
    ${src}/core/plays/StopReferee.cpp
    ${src}/core/plays/ObeyReferee.cpp
    ${src}/core/plays/AutoRetaliate.cpp
    ${src}/core/plays/PenaltyUs.cpp
    ${src}/core/plays/PenaltyThem.cpp
    ${src}/core/plays/IndirectKick.cpp
    # add new sources here

    ${src}/core/skills/Wait.cpp
    ${src}/core/skills/DriveTo.cpp
    ${src}/core/skills/SteerToPoint.cpp
    ${src}/core/skills/DriveToBall.cpp
    ${src}/core/skills/Steer.cpp
    ${src}/core/skills/DriveToObject.cpp
    ${src}/core/skills/Sleep.cpp
    ${src}/core/skills/ForceFieldMotion.cpp
    ${src}/core/skills/Skill.cpp
    ${src}/core/skills/GetBall.cpp
    ${src}/core/skills/SampledKick.cpp
    ${src}/core/skills/Goto.cpp
    ${src}/core/skills/SampledDribble.cpp
    ${src}/core/skills/GotoOld.cpp
    ${src}/core/skills/Move.cpp
    ${src}/core/skills/KickTo.cpp
    ${src}/core/skills/Loops.cpp
    ${src}/core/skills/LinearCombMotion.cpp
    ${src}/core/skills/GotoAvoid.cpp
    ${src}/core/skills/FollowAndCover.cpp
    # add new sources here

    ${src}/core/tactics/GotoTactic.cpp
    ${src}/core/tactics/Attacker.cpp
    ${src}/core/tactics/Zickler43.cpp
    ${src}/core/tactics/Blocker.cpp
    ${src}/core/tactics/Tactic.cpp
    ${src}/core/tactics/LinearCombTactic.cpp
    ${src}/core/tactics/KeyboardController.cpp
    ${src}/core/tactics/Defender.cpp
    ${src}/core/tactics/GoalSwitcheroo.cpp
    ${src}/core/tactics/Goalkeeper.cpp
    # add new sources here

    ${src}/interface/UpdateReferee.cpp
    ${src}/interface/UpdateStageGeometry.cpp
    ${src}/interface/UpdateRobot.cpp
    ${src}/interface/Updates.cpp
    ${src}/interface/UpdateBall.cpp
    # add new sources here

    ${src}/interface/base/Command.cpp
    ${src}/interface/base/Update.cpp
    ${src}/interface/base/Commander.cpp
    ${src}/interface/base/Updater.cpp
    ${src}/interface/base/UdpServer.cpp
    # add new sources here

    ${src}/interface/commanders/CommanderTxOld.cpp
    ${src}/interface/commanders/CommanderTx.cpp
    ${src}/interface/commanders/CommanderSim.cpp
    ${src}/interface/commanders/CommanderGrSim.cpp
    # add new sources here

    ${src}/interface/updaters/UpdaterReferee.cpp
    ${src}/interface/updaters/UpdaterVision.cpp
    # add new sources here

    ${src}/prototypes/Robots.cpp
    # add new sources here

    ${src}/utils/PID.cpp
    ${src}/utils/LinearRegression.cpp
    ${src}/utils/Sampler.cpp
    # add new sources here
)
if(MSVC)
set(INT_SRC
    ${INT_SRC}
    ${src}/utils/CXBOXController.cpp
    ${src}/core/plays/MultiController.cpp
    ${src}/core/tactics/Controller.cpp
    ${src}/core/tactics/Controller2.cpp
)
endif()

aux_source_directory(${src}/utils/tree INT_SRC_UTILS_TREE)

##############################
# Protobuf files
#

set(COMM_PROTO
    ${src}/interface/messages/messages_internal_command.proto
    ${src}/interface/messages/messages_robocup_ssl_detection.proto
    ${src}/interface/messages/messages_robocup_ssl_geometry.proto
    ${src}/interface/messages/messages_robocup_ssl_refbox_log.proto
    ${src}/interface/messages/messages_robocup_ssl_wrapper.proto
    ${src}/interface/messages/grSim_Commands.proto
    ${src}/interface/messages/grSim_Replacement.proto
    ${src}/interface/messages/grSim_Packet.proto
    # add new protos here
)

protobuf_generate_cpp(INT_PROTO_SRC INT_PROTO_HDR ${COMM_PROTO})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


##############################
# Le finale
#

# adding to group
directory_source_group(headers . *.h)
directory_source_group(src . *.cpp)
directory_source_group(proto . *.proto)
source_group(generated REGULAR_EXPRESSION ".*_automoc[.]cpp")

foreach(DIR ${INT_INC})
    set(INC_INC_DIRS
        ${INC_INC_DIRS}
        "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}/${DIR}"
    )
endforeach()

set(INT_INC_DIRS ${INC_INC_DIRS})

set(lib roboime-intel)

add_library(${lib}
    ${INT_PROTO_SRC}
    ${INT_PROTO_HDR}
    ${INT_HDR}
    ${INT_SRC}
)

target_link_libraries(${lib}
    ${PROTOBUF_LIBRARIES}
)

qt5_use_modules(${lib} Core Network Widgets)

set_property(TARGET ${lib} PROPERTY FOLDER modules)


##############################
# Optionally build clients.
#

option(BUILD_CLI "Build command line client?" ON)
if(BUILD_CLI)
    add_subdirectory(clients/cli)
endif()
option(BUILD_GUI "Build graphical client?" ON)
if(BUILD_GUI)
    add_subdirectory(clients/gui)
endif()

