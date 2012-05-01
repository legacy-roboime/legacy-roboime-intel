cmake_minimum_required(VERSION 2.8)

##############################
# Files which need to be moc'd
# Ideally this should be automatic, but it isn't yet
#

set(LOGP_MOC
    LogPlayer.h
    # add new headers here
)

##############################
# Qt Form files (.ui)
#

set(LOGP_UI
	LogPlayer.ui
	# add new uis here
)
source_group("Forms" FILES ${LOGP_UI})

##############################
# Qt resources
#

set(LOGP_RES
	LogPlayer.qrc
	# add new resources here
)
source_group("Resources" FILES ${LOGP_RES})

##############################
# Include all headers
#

include_directories(.)
file(GLOB LOGP_HDR "*.h")
source_group("Headers" FILES ${LOGP_HDR})

##############################
# Files which will be compiled
#

set(LOGP_SRC
    main.cpp
    LogPlayer.cpp
    # add new sources here
)
source_group("Sources" FILES ${LOGP_SRC})



##############################
# Intra dependencies
#

# Simulation
include_directories(${SIM_INC_DIRS})



##############################
# Qt stuff
#

find_package(Qt4 REQUIRED)
set(QT_USE_QTNETWORK true)
set(QT_USE_QTOPENGL true)

qt4_wrap_cpp(LOGP_MOC_CPP ${LOGP_MOC})
qt4_wrap_ui(LOGP_UI_H ${LOGP_UI})
qt4_add_resources(LOGP_RES_CPP ${LOGP_RES})

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
    ${LOGP_MOC_CPP}
    ${LOGP_UI_H}
    ${LOGP_RES_CPP}
)

# adding to group
source_group("Generated" FILES ${GEN_FILES})

add_executable(logplayer
    ${LOGP_HDR}
    ${LOGP_SRC}
    ${LOGP_UI}
    ${LOGP_RES}
    ${GEN_FILES}
)

target_link_libraries(logplayer
    simulation
    samplescommoncode
    ${QT_LIBRARIES}
    ${PHYSX_LIBRARIES}
)
