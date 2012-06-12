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
    base/StageX.cpp
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

    core/plays/BGT.cpp
    core/plays/Play.cpp
    core/plays/CBR2011.cpp
    core/plays/MultiController.cpp
    core/plays/FreeKickThem.cpp
    core/plays/Minmax.cpp
    core/plays/Minmax2.cpp
    core/plays/Halt.cpp
    core/plays/LinearCombPlay.cpp
    core/plays/StopReferee.cpp
    core/plays/ObeyReferee.cpp
    # add new sources here

    core/skills/AttackField.cpp
    core/skills/Wait.cpp
    core/skills/DriveTo.cpp
    core/skills/SteerToBall.cpp
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
    # add new sources here

    core/tactics/GotoTactic.cpp
    core/tactics/Attacker.cpp
    core/tactics/AttackerMinMax2.cpp
    core/tactics/Zickler43.cpp
    core/tactics/Blocker.cpp
    core/tactics/Tactic.cpp
    core/tactics/Controller.cpp
    core/tactics/LinearCombTactic.cpp
    core/tactics/Controller2.cpp
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
    # add new sources here

    interface/commanders/CommanderTxOld.cpp
    interface/commanders/CommanderTx.cpp
    interface/commanders/CommanderSim.cpp
    # add new sources here

    interface/updaters/UpdaterReferee.cpp
    interface/updaters/UpdaterSIMi.cpp
    interface/updaters/UpdaterSIM.cpp
    interface/updaters/UpdaterVision.cpp
    # add new sources here

    prototypes/Robots.cpp
    # add new sources here

    utils/CXBOXController.cpp
    utils/PID.cpp
    utils/LinearRegression.cpp
    utils/Sampler.cpp
    # add new sources here
)

aux_source_directory(utils/tree INT_SRC_UTILS_TREE)


##############################
# Intra dependencies
#

# Simulation
include_directories(${SIM_INC_DIRS})

# Soccer
include_directories(${SOC_INC_DIRS})

# Communication
include_directories(${COMM_INC_DIRS})

# Kalman
find_library(KALMAN_LIBRARY "libKalman.lib" "${PROJECT_SOURCE_DIR}/../lib")
find_library(KALMAN_LIBRARY_DEBUG "libKalmand.lib" "${PROJECT_SOURCE_DIR}/../lib")



##############################
# Qt stuff
#

set(QT_DONT_USE_QTGUI true)
set(QT_USE_QTNETWORK true)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})



##############################
# Le finale
#

# adding to group
directory_source_group(headers . *.h)
directory_source_group(src . *.cpp)
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
    ${COMM_INC_DIRS}
    ${SOC_INC_DIRS}
    ${SIM_INC_DIRS}
    PARENT_SCOPE
)

add_library(intelligence
    ${GEN_FILES}
    ${INT_HDR}
    ${INT_SRC}
)

target_link_libraries(intelligence
    ${QT_LIBRARIES}
    ${KALMAN_LIBRARY}
    simulation
    soccer
    communication
)

set_property(TARGET intelligence PROPERTY FOLDER modules)
