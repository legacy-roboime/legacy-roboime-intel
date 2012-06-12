cmake_minimum_required(VERSION 2.8.8)

##############################
# Qt Form files (.ui)
#

set(LOGP_UI
	LogPlayer.ui
	# add new uis here
)

##############################
# Qt resources
#

set(LOGP_RES
	LogPlayer.qrc
	# add new resources here
)

##############################
# Include all headers
#

include_directories(.)
file(GLOB LOGP_HDR "*.h")

##############################
# Files which will be compiled
#

set(LOGP_SRC
    main.cpp
    LogPlayer.cpp
    # add new sources here
)



##############################
# Intra dependencies
#

# Simulation
include_directories(${SIM_INC_DIRS})



##############################
# Qt stuff
#

set(QT_USE_QTNETWORK true)
set(QT_USE_QTOPENGL true)
find_package(Qt4 REQUIRED)

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
    ${LOGP_UI_H}
    ${LOGP_RES_CPP}
)

# adding to group
source_group("Generated" FILES ${GEN_FILES})
directory_source_group(headers . *.h)
directory_source_group(src . *.cpp)
directory_source_group(res . *.qrc)
directory_source_group(forms . *.ui)
source_group(generated REGULAR_EXPRESSION ".*_automoc[.]cpp")

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

set_property(TARGET logplayer PROPERTY FOLDER apps)
