cmake_minimum_required(VERSION 2.8)

##############################
# Include all headers
#

include_directories(.)
file(GLOB GINTC_HDR "*.h")

##############################
# Files which will be compiled
#

set(GINTC_SRC
    main.cpp
    GraphicalIntelligence.cpp
    itemfield.cpp
    itemrobot.cpp
    stageview.cpp
    alterstatevars.cpp
    # add new sources here
)

qt4_wrap_ui(GINTC_UIS_H
    GraphicalIntelligence.ui
    alterstatevars.ui
)

qt4_add_resources(GINTC_RC
    GraphicalIntelligence.qrc
    ${SHARED_RC}
)


qt4_wrap_cpp(GINTC_MOC_SRCS
    GraphicalIntelligence.h
)

##############################
# Intra dependencies
#

# Intelligence
include_directories(${INT_INC_DIRS})

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

##############################
# Qt stuff
#

set(QT_USE_QTNETWORK true)
set(QT_USE_QTOPENGL true)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})



##############################
# Le finale
#

# adding to group
directory_source_group(headers . *.h)
directory_source_group(src . *.cpp)
source_group(generated REGULAR_EXPRESSION ".*_automoc[.]cpp")

#Now add these generated files to the ADD_EXECUTABLE step
# If this is NOT done, then the ui_*.h files will not be generated
add_executable(graphicalintelligence-client
    ${GINTC_HDR}
    ${GINTC_SRC}
    ${GINTC_UIS_H}
    ${GINTC_RC}
    #${GINTC_MOC_SRCS}
    ${GINTC_UIS_H}
    ${GINTC_RC}
)

target_link_libraries(graphicalintelligence-client
    intelligence
    ${GLUT_LIBRARIES}
    ${QT_LIBRARIES}
)

set_property(TARGET graphicalintelligence-client PROPERTY FOLDER apps)

