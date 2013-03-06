cmake_minimum_required(VERSION 2.8)

##############################
# Include all headers
#

set(INT_INC . base core core/filters core/plays core/skills core/tactics interface interface/base interface/commanders interface/updaters prototypes utils utils/tree utils/kalman)
include_directories(${INT_INC})

file(GLOB_RECURSE INT_HDR "*.h")

##############################
# Files which will be compiled
#

set(INT_SRC
    base/Actuator.cpp
    base/Team.cpp
    base/Ball.cpp
    base/StateMachine.cpp
    base/Battery.cpp
    base/State.cpp
    base/Body.cpp
    base/StageY.cpp
    base/Component.cpp
    base/Dribbler.cpp
    base/Stage.cpp
    base/Goal.cpp
    base/Robot.cpp
    base/Kicker.cpp
    base/Object.cpp
    base/MachineTransition.cpp
    base/Wheel.cpp
    # add new sources here

    core/filters/KalmanFilters.cpp
    core/filters/KalmanFilter.cpp
    core/filters/Filter.cpp
    # add new sources here

    core/plays/Play.cpp
    core/plays/CBR2011.cpp
    core/plays/FreeKickThem.cpp
    core/plays/Halt.cpp
    core/plays/LinearCombPlay.cpp
    core/plays/StopReferee.cpp
    core/plays/ObeyReferee.cpp
    core/plays/AutoRetaliate.cpp
    core/plays/PenaltyUs.cpp
    core/plays/PenaltyThem.cpp
    core/plays/IndirectKick.cpp
    # add new sources here

    core/skills/Wait.cpp
    core/skills/DriveTo.cpp
    core/skills/SteerToPoint.cpp
    core/skills/DriveToBall.cpp
    core/skills/Steer.cpp
    core/skills/DriveToObject.cpp
    core/skills/Sleep.cpp
    core/skills/ForceFieldMotion.cpp
    core/skills/Skill.cpp
    core/skills/GetBall.cpp
    core/skills/SampledKick.cpp
    core/skills/Goto.cpp
    core/skills/SampledDribble.cpp
    core/skills/GotoOld.cpp
    core/skills/Move.cpp
    core/skills/KickTo.cpp
    core/skills/Loops.cpp
    core/skills/LinearCombMotion.cpp
    core/skills/GotoAvoid.cpp
    core/skills/FollowAndCover.cpp
    # add new sources here

    core/tactics/GotoTactic.cpp
    core/tactics/Attacker.cpp
    core/tactics/Zickler43.cpp
    core/tactics/Blocker.cpp
    core/tactics/Tactic.cpp
    core/tactics/LinearCombTactic.cpp
    core/tactics/KeyboardController.cpp
    core/tactics/Defender.cpp
    core/tactics/GoalSwitcheroo.cpp
    core/tactics/Goalkeeper.cpp
    # add new sources here

    interface/UpdateReferee.cpp
    interface/UpdateStageGeometry.cpp
    interface/UpdateRobot.cpp
    interface/Updates.cpp
    interface/UpdateBall.cpp
    # add new sources here

    interface/base/Command.cpp
    interface/base/Update.cpp
    interface/base/Commander.cpp
    interface/base/Updater.cpp
    interface/base/UdpServer.cpp
    # add new sources here

    interface/commanders/CommanderTxOld.cpp
    interface/commanders/CommanderTx.cpp
    interface/commanders/CommanderSim.cpp
    interface/commanders/CommanderGrSim.cpp
    # add new sources here

    interface/updaters/UpdaterReferee.cpp
    interface/updaters/UpdaterVision.cpp
    # add new sources here

    prototypes/Robots.cpp
    # add new sources here

    utils/PID.cpp
    utils/LinearRegression.cpp
    utils/Sampler.cpp
    # add new sources here
)
if(MSVC)
set(INT_SRC
    ${INT_SRC}
    utils/CXBOXController.cpp
    core/plays/MultiController.cpp
    core/tactics/Controller.cpp
    core/tactics/Controller2.cpp
)
endif()

aux_source_directory(utils/tree INT_SRC_UTILS_TREE)

##############################
# Protobuf files
#

set(COMM_PROTO
    interface/messages/messages_internal_command.proto
    interface/messages/messages_robocup_ssl_detection.proto
    interface/messages/messages_robocup_ssl_geometry.proto
    interface/messages/messages_robocup_ssl_refbox_log.proto
    interface/messages/messages_robocup_ssl_wrapper.proto
    interface/messages/grSim_Commands.proto
    interface/messages/grSim_Replacement.proto
    interface/messages/grSim_Packet.proto
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

set(INT_INC_DIRS
    ${INC_INC_DIRS}
    PARENT_SCOPE
)

add_library(intelligence
    #${GEN_FILES}
    ${INT_PROTO_SRC}
    ${INT_PROTO_HDR}
    ${INT_HDR}
    ${INT_SRC}
)

target_link_libraries(intelligence
    ${PROTOBUF_LIBRARIES}
)

qt5_use_modules(intelligence
    Core
    Network
    Widgets
)

set_property(TARGET intelligence PROPERTY FOLDER modules)

