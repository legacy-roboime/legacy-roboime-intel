cmake_minimum_required(VERSION 2.8)

##############################
# Files which need to be moc'd
#

# for convenience we moc EVERYTHING
# that's not perfect, but it's not that bad either
file(GLOB_RECURSE INT_MOC "*.h")

##############################
# Include all headers
#

set(INT_INC . base core core/filters core/plays core/skills core/tactics interface interface/base interface/commanders interface/updaters prototypes utils utils/tree utils/kalman)
include_directories(${INT_INC})

file(GLOB INT_HDR "base/*.h")
source_group("Headers\\Base" FILES ${INT_HDR})

file(GLOB INT_HDR "core/*.h")
source_group("Headers\\Core" FILES ${INT_HDR})

file(GLOB INT_HDR "core/filters/*.h")
source_group("Headers\\Core\\Filters" FILES ${INT_HDR})

file(GLOB INT_HDR "core/plays*.h")
source_group("Headers\\Core\\Plays" FILES ${INT_HDR})

file(GLOB INT_HDR "core/skills*.h")
source_group("Headers\\Core\\Skills" FILES ${INT_HDR})

file(GLOB INT_HDR "core/tactics*.h")
source_group("Headers\\Core\\Tactics" FILES ${INT_HDR})

file(GLOB INT_HDR "interface/*.h")
source_group("Headers\\Interface" FILES ${INT_HDR})

file(GLOB INT_HDR "interface/base*.h")
source_group("Headers\\Interface\\Base" FILES ${INT_HDR})

file(GLOB INT_HDR "interface/commanders/*.h")
source_group("Headers\\Interface\\Commanders" FILES ${INT_HDR})

file(GLOB INT_HDR "interface/updaters/*.h")
source_group("Headers\\Interface\\Updaters" FILES ${INT_HDR})

file(GLOB INT_HDR "prototypes/*.h")
source_group("Headers\\Prototypes" FILES ${INT_HDR})

file(GLOB INT_HDR "utils/*.h")
source_group("Headers\\Utils" FILES ${INT_HDR})

file(GLOB INT_HDR "*.h")
source_group("Headers" FILES ${INT_HDR})

file(GLOB_RECURSE INT_HDR "*.h")

##############################
# Files which will be compiled
#

set(INT_SRC_BASE
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
)
source_group("Sources\\Base" FILES ${INT_SRC_BASE})

set(INT_SRC_CORE_FILTERS
    core/filters/KalmanFilters.cpp
    core/filters/KalmanFilter.cpp
    core/filters/Filter.cpp
    # add new sources here
)
source_group("Sources\\Core\\Filters" FILES ${INT_SRC_CORE_FILTERS})

set(INT_SRC_CORE_PLAYS
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
    # add new sources here
)
source_group("Sources\\Core\\Plays" FILES ${INT_SRC_CORE_PLAYS})

set(INT_SRC_CORE_SKILLS
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
)
source_group("Sources\\Core\\Skills" FILES ${INT_SRC_CORE_SKILLS})

set(INT_SRC_CORE_TACTICS
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
)
source_group("Sources\\Core\\Tactics" FILES ${INT_SRC_CORE_TACTICS})

set(INT_SRC_INTERFACE
    interface/UpdateReferee.cpp
    interface/UpdateStageGeometry.cpp
    interface/UpdateRobot.cpp
    interface/Updates.cpp
    interface/UpdateBall.cpp
    # add new sources here
)
source_group("Sources\\Interface" FILES ${INT_SRC_INTERFACE})

set(INT_SRC_INTERFACE_BASE
    interface/base/Command.cpp
    interface/base/Update.cpp
    interface/base/Commander.cpp
    interface/base/Updater.cpp
    # add new sources here
)
source_group("Sources\\Interface\\Base" FILES ${INT_SRC_INTERFACE_BASE})

set(INT_SRC_INTERFACE_COMMANDERS
    interface/commanders/CommanderTxOld.cpp
    interface/commanders/CommanderTx.cpp
    interface/commanders/CommanderSim.cpp
    # add new sources here
)
source_group("Sources\\Interface\\Commanders" FILES ${INT_SRC_INTERFACE_COMMANDERS})

set(INT_SRC_INTERFACE_UPDATERS
    interface/updaters/UpdaterReferee.cpp
    interface/updaters/UpdaterSIMi.cpp
    interface/updaters/UpdaterSIM.cpp
    interface/updaters/UpdaterVision.cpp
    # add new sources here
)
source_group("Sources\\Interface\\Updaters" FILES ${INT_SRC_INTERFACE_UPDATERS})

set(INT_SRC_PROTOTYPES
    prototypes/Robots.cpp
    # add new sources here
)
source_group("Sources\\Prototypes" FILES ${INT_SRC_PROTOTYPES})

set(INT_SRC_UTILS
    utils/CXBOXController.cpp
    utils/PID.cpp
    utils/LinearRegression.cpp
    utils/Sampler.cpp
    # add new sources here
)
source_group("Sources\\Utils" FILES ${INT_SRC_UTILS})

aux_source_directory(utils/tree INT_SRC_UTILS_TREE)
source_group("Sources\\Utils\\Tree" FILES ${INT_SRC_UTILS_TREE})

set(INT_SRC
    ${INT_SRC_BASE}
    ${INT_SRC_CORE_FILTERS}
    ${INT_SRC_CORE_PLAYS}
    ${INT_SRC_CORE_SKILLS}
    ${INT_SRC_CORE_TACTICS}
    ${INT_SRC_INTERFACE}
    ${INT_SRC_INTERFACE_BASE}
    ${INT_SRC_INTERFACE_COMMANDERS}
    ${INT_SRC_INTERFACE_UPDATERS}
    ${INT_SRC_PROTOTYPES}
    ${INT_SRC_UTILS}
    ${INT_SRC_UTILS_TREE}
)



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

find_package(Qt4 REQUIRED)
set(QT_DONT_USE_QTGUI true)
set(QT_USE_QTNETWORK true)
include(${QT_USE_FILE})
qt4_wrap_cpp(INT_MOC_CPP ${INT_MOC})



##############################
# Le finale
#

set(GEN_FILES
    ${INT_MOC_CPP}
)

# adding to group
source_group("Generated" FILES ${GEN_FILES})

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
