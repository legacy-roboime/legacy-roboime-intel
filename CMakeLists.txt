cmake_minimum_required(VERSION 2.8.9)

project(roboime-intel)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
include(${PROJECT_SOURCE_DIR}/cmake/Utils.cmake)

standard_config()
standard_paths(${PROJECT_SOURCE_DIR} bin lib)
#message(STATUS "Project src dir: ${PROJECT_SOURCE_DIR}")

# Qt stuff
find_package(Qt5 REQUIRED)

#add_definitions(-DUSE_PHYSX)
#add_definitions(-DUSE_SIMULATION)

# Protobuf stuff
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

# Set where the source is contained.
set(src src)

##############################
# Source files
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
    ${src}/utils/tree
)
include_directories(${INT_INC})


#file(GLOB_RECURSE INT_HDR RELATIVE ${src} "*.h")
set(INT_HDR
    ${src}/config.h
    ${src}/LibIntelligence.h

    ${src}/base/Actuator.h
    ${src}/base/Ball.h
    ${src}/base/Battery.h
    ${src}/base/Body.h
    ${src}/base/Component.h
    ${src}/base/Dribbler.h
    ${src}/base/Goal.h
    ${src}/base/Kicker.h
    ${src}/base/MachineTransition.h
    ${src}/base/Object.h
    ${src}/base/Robot.h
    ${src}/base/Stage.h
    ${src}/base/StageY.h
    ${src}/base/State.h
    ${src}/base/StateMachine.h
    ${src}/base/Team.h
    ${src}/base/Wheel.h

    ${src}/core/filters/Filter.h
    ${src}/core/filters/KalmanFilter.h
    ${src}/core/filters/KalmanFilters.h

    ${src}/core/plays/AutoRetaliate.h
    ${src}/core/plays/CBR2011.h
    ${src}/core/plays/FreeKickThem.h
    ${src}/core/plays/Halt.h
    ${src}/core/plays/IndirectKick.h
    ${src}/core/plays/LinearCombPlay.h
    ${src}/core/plays/ObeyReferee.h
    ${src}/core/plays/PenaltyThem.h
    ${src}/core/plays/PenaltyUs.h
    ${src}/core/plays/Play.h
    ${src}/core/plays/Plays.h
    ${src}/core/plays/StopReferee.h

    ${src}/core/skills/DriveTo.h
    ${src}/core/skills/DriveToBall.h
    ${src}/core/skills/DriveToObject.h
    ${src}/core/skills/FollowAndCover.h
    ${src}/core/skills/ForceFieldMotion.h
    ${src}/core/skills/GetBall.h
    ${src}/core/skills/Goto.h
    ${src}/core/skills/GotoAvoid.h
    ${src}/core/skills/GotoOld.h
    ${src}/core/skills/KickTo.h
    ${src}/core/skills/LinearCombMotion.h
    ${src}/core/skills/Loops.h
    ${src}/core/skills/Move.h
    ${src}/core/skills/SampledDribble.h
    ${src}/core/skills/SampledKick.h
    ${src}/core/skills/Skill.h
    ${src}/core/skills/Skills.h
    ${src}/core/skills/Sleep.h
    ${src}/core/skills/Steer.h
    ${src}/core/skills/SteerToPoint.h
    ${src}/core/skills/Wait.h

    ${src}/core/tactics/Attacker.h
    ${src}/core/tactics/Blocker.h
    ${src}/core/tactics/Defender.h
    ${src}/core/tactics/Goalkeeper.h
    ${src}/core/tactics/GoalSwitcheroo.h
    ${src}/core/tactics/GotoTactic.h
    ${src}/core/tactics/KeyboardController.h
    ${src}/core/tactics/LinearCombTactic.h
    ${src}/core/tactics/Tactic.h
    ${src}/core/tactics/Tactics.h
    ${src}/core/tactics/Zickler43.h

    ${src}/interface/base/Command.h
    ${src}/interface/base/Commander.h
    ${src}/interface/base/UdpServer.h
    ${src}/interface/base/Update.h
    ${src}/interface/base/Updater.h

    ${src}/interface/commanders/CommanderGrSim.h
    ${src}/interface/commanders/CommanderSim.h
    ${src}/interface/commanders/CommanderSIMi.h
    ${src}/interface/commanders/CommanderTx.h
    ${src}/interface/commanders/CommanderTxOld.h

    ${src}/interface/UpdateBall.h
    ${src}/interface/UpdateReferee.h
    ${src}/interface/UpdateRobot.h
    ${src}/interface/updaters/UpdaterReferee.h
    ${src}/interface/updaters/UpdaterVision.h
    ${src}/interface/Updates.h
    ${src}/interface/UpdateStageGeometry.h

    ${src}/prototypes/Robots.h

    ${src}/utils/geomutils.h
    ${src}/utils/LinearRegression.h
    ${src}/utils/mathutils.h
    ${src}/utils/PID.h
    ${src}/utils/Sampler.h
)
if(MSVC)
set(INT_HDR
    ${INT_HDR}
    ${src}/utils/CXBOXController.h
    ${src}/core/plays/MultiController.h
    ${src}/core/tactics/Controller.h
    ${src}/core/tactics/Controller2.h
)
endif()


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

    ${src}/core/filters/KalmanFilters.cpp
    ${src}/core/filters/KalmanFilter.cpp
    ${src}/core/filters/Filter.cpp

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

    ${src}/interface/UpdateReferee.cpp
    ${src}/interface/UpdateStageGeometry.cpp
    ${src}/interface/UpdateRobot.cpp
    ${src}/interface/Updates.cpp
    ${src}/interface/UpdateBall.cpp

    ${src}/interface/base/Command.cpp
    ${src}/interface/base/Update.cpp
    ${src}/interface/base/Commander.cpp
    ${src}/interface/base/Updater.cpp
    ${src}/interface/base/UdpServer.cpp

    ${src}/interface/commanders/CommanderTxOld.cpp
    ${src}/interface/commanders/CommanderTx.cpp
    ${src}/interface/commanders/CommanderSim.cpp
    ${src}/interface/commanders/CommanderGrSim.cpp

    ${src}/interface/updaters/UpdaterReferee.cpp
    ${src}/interface/updaters/UpdaterVision.cpp

    ${src}/prototypes/Robots.cpp

    ${src}/utils/PID.cpp
    ${src}/utils/LinearRegression.cpp
    ${src}/utils/Sampler.cpp
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
set(INT_SRC ${INT_SRC} ${INT_SRC_UTILS_TREE})

set(INT_PROTO
    ${src}/interface/messages/messages_internal_command.proto
    ${src}/interface/messages/messages_robocup_ssl_detection.proto
    ${src}/interface/messages/messages_robocup_ssl_geometry.proto
    ${src}/interface/messages/messages_robocup_ssl_refbox_log.proto
    ${src}/interface/messages/messages_robocup_ssl_wrapper.proto
    ${src}/interface/messages/grSim_Commands.proto
    ${src}/interface/messages/grSim_Replacement.proto
    ${src}/interface/messages/grSim_Packet.proto
)

protobuf_generate_cpp(INT_PROTO_SRC INT_PROTO_HDR ${INT_PROTO})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


##############################
# Le finale
#

foreach(DIR ${INT_INC})
    set(INC_INC_DIRS
        ${INC_INC_DIRS}
        "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}/${DIR}"
    )
endforeach()

set(INT_INC_DIRS ${INC_INC_DIRS})

set(lib roboime-intel)

qt5_wrap_cpp(INT_MOC_SRC ${INT_HDR} OPTIONS -nn)

add_library(${lib}
    ${INT_PROTO_SRC}
    ${INT_PROTO_HDR}
    ${INT_MOC_SRC}
    ${INT_HDR}
    ${INT_SRC}
)

target_link_libraries(${lib}
    ${PROTOBUF_LIBRARIES}
)

qt5_use_modules(${lib} Core Network Widgets)

set_property(TARGET ${lib} PROPERTY FOLDER modules)

# adding to group
directory_source_group(src ${src}${src} *.cc)
directory_source_group(src ${src}${src} *.cxx)
directory_source_group(src ${src}${src} *.cpp)
directory_source_group(headers ${src} *.h)
directory_source_group(headers ${src} *.hh)
directory_source_group(headers ${src} *.hpp)
directory_source_group(proto ${src} *.proto)
source_group(generated REGULAR_EXPRESSION ".*_automoc[.]cpp")
source_group(generated REGULAR_EXPRESSION ".*.pb.cc")
source_group(generated REGULAR_EXPRESSION ".*.pb.hh")


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

