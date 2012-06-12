cmake_minimum_required(VERSION 2.8.8)

##############################
# Qt Form files (.ui)
#

set(SIMC_UI
	SimulationView.ui
	# add new uis here
)

##############################
# Qt resources
#

set(SIMC_RES
	SimulationView.qrc
	# add new resources here
)

##############################
# Include all headers
#

include_directories(.)
file(GLOB SIMC_HDR "*.h")

##############################
# Files which will be compiled
#

set(SIMC_SRC
    main.cpp
    GLWidget.cpp
    SimulationView.cpp
    # add new sources here
)



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

set(QT_USE_QTNETWORK true)
set(QT_USE_QTOPENGL true)
find_package(Qt4 REQUIRED)

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
    ${SIMC_UI_H}
    ${SIMC_RES_CPP}
)

# adding to group
source_group("Generated" FILES ${GEN_FILES})
directory_source_group(headers . *.h)
directory_source_group(src . *.cpp)
directory_source_group(res . *.qrc)
directory_source_group(forms . *.ui)
source_group(generated REGULAR_EXPRESSION ".*_automoc[.]cpp")

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

set_property(TARGET simulation-client PROPERTY FOLDER apps)
