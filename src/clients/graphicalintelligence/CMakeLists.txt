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
    graphical.rc
    main.cpp
    GraphicalIntelligence.cpp
    itemfield.cpp
    itemrobot.cpp
    stageview.cpp
    alterstatevars.cpp
    # add new sources here
    GraphicalIntelligence.ui
    alterstatevars.ui
    GraphicalIntelligence.qrc
    ${SHARED_RC}
)

set(GINTC_UI
    GraphicalIntelligence.ui
    alterstatevars.ui
)

set(GINT_RC
    GraphicalIntelligence.qrc
    ${SHARED_RC}
)

#qt5_wrap_ui(GINTC_UIS_H
#    GraphicalIntelligence.ui
#    alterstatevars.ui
#)
#
#qt5_add_resources(GINTC_RC
#    GraphicalIntelligence.qrc
#    ${SHARED_RC}
#)
#
#qt5_wrap_cpp(GINTC_MOC_SRCS
#    GraphicalIntelligence.h
#)

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

find_package(Qt5Core)
find_package(Qt5Widgets)

qt5_wrap_ui(GINTC_UIS_H ${GINTC_UI})
qt5_add_resources(GINTC_RC_CXX ${GINTC_RC})


##############################
# Le finale
#

# adding to group
directory_source_group(headers . *.h)
directory_source_group(src . *.cpp)
source_group(generated REGULAR_EXPRESSION ".*_automoc[.]cpp")

#Now add these generated files to the ADD_EXECUTABLE step
# If this is NOT done, then the ui_*.h files will not be generated
add_executable(graphicalintelligence-client WIN32
    ${GINTC_HDR}
    ${GINTC_SRC}
    ${GINTC_UIS_H}
    ${GINTC_RC_CXX}
    #${GINTC_MOC_SRCS}
)

target_link_libraries(graphicalintelligence-client
    intelligence
)

qt5_use_modules(graphicalintelligence-client
    Core
    Widgets
    OpenGL
    Gui
)

set_property(TARGET graphicalintelligence-client PROPERTY FOLDER apps)

