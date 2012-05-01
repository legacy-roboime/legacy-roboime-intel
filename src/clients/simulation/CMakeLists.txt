cmake_minimum_required(VERSION 2.8)

##############################
# Files which need to be moc'd
# Ideally this should be automatic, but it isn't yet
#

set(SIMC_MOC
    SimulationView.h
    # add new headers here
)

##############################
# Qt Form files (.ui)
#

set(SIMC_UI
	SimulationView.ui
	# add new uis here
)
source_group("Forms" FILES ${SIMC_UI})

##############################
# Qt resources
#

set(SIMC_RES
	SimulationView.qrc
	# add new resources here
)
source_group("Resources" FILES ${SIMC_RES})

##############################
# Include all headers
#

include_directories(.)
file(GLOB SIMC_HDR "*.h")
source_group("Headers" FILES ${SIMC_HDR})

##############################
# Files which will be compiled
#

set(SIMC_SRC
    main.cpp
    GLWidget.cpp
    SimulationView.cpp
    # add new sources here
)
source_group("Sources" FILES ${SIMC_SRC})



##############################
# Intra dependencies
#

# Simulation
include_directories(${SIM_INC_DIRS})



##############################
# GLUT stuff
#

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})



##############################
# Qt stuff
#

find_package(Qt4 REQUIRED)
set(QT_USE_QTNETWORK true)
set(QT_USE_QTOPENGL true)

qt4_wrap_cpp(SIMC_MOC_CPP ${SIMC_MOC})
qt4_wrap_ui(SIMC_UI_H ${SIMC_UI})
qt4_add_resources(SIMC_RES_CPP ${SIMC_RES})

include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_BINARY_DIR})



##############################
# PhysX stuff
#

find_package(PhysX REQUIRED)
include_directories(${PHYSX_INCLUDES})



##############################
# Le finale
#

set(GEN_FILES
    ${SIMC_MOC_CPP}
    ${SIMC_UI_H}
    ${SIMC_RES_CPP}
)

# adding to group
source_group("Generated" FILES ${GEN_FILES})

add_executable(simulation-client
    ${SIMC_HDR}
    ${SIMC_SRC}
    ${SIMC_UI}
    ${SIMC_RES}
    ${GEN_FILES}
)

target_link_libraries(simulation-client
    simulation
    samplescommoncode
    communication
    ${QT_LIBRARIES}
    ${PHYSX_LIBRARIES}
    ${GLUT_LIBRARIES}
)
