cmake_minimum_required(VERSION 2.8.9)

project(roboime-gui)

##############################
# Source files
#

set(src src)
include_directories(${src})

set(GUI_HDR
    ${src}/alterstatevars.h
    ${src}/GraphicalIntelligence.h
    ${src}/itemfield.h
    ${src}/itemrobot.h
    ${src}/stageview.h
    ${src}/winmain.h
)

set(GUI_SRC
    ${src}/graphical.rc
    ${src}/main.cpp
    ${src}/GraphicalIntelligence.cpp
    ${src}/itemfield.cpp
    ${src}/itemrobot.cpp
    ${src}/stageview.cpp
    ${src}/alterstatevars.cpp
)

set(GUI_UI
    ${src}/GraphicalIntelligence.ui
    ${src}/alterstatevars.ui
)

set(GUI_RC
    ${SHARED_RC}
    ${src}/GraphicalIntelligence.qrc
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

find_package(Qt5Core)
find_package(Qt5Widgets)

qt5_wrap_ui(GUI_UIS_H ${GUI_UI})
qt5_add_resources(GUI_RC_CXX ${GUI_RC})


##############################
# Le finale
#

# adding to group
directory_source_group(headers ${src} *.h)
directory_source_group(headers ${src} *.hh)
directory_source_group(headers ${src} *.hpp)
directory_source_group(src ${src} *.cc)
directory_source_group(src ${src} *.cxx)
directory_source_group(src ${src} *.cpp)
source_group(generated REGULAR_EXPRESSION ".*_automoc[.]cpp")

set(app roboime-gui)

qt5_wrap_cpp(GUI_MOC_SRC ${GUI_HDR} OPTIONS -nn)

#Now add these generated files to the ADD_EXECUTABLE step
# If this is NOT done, then the ui_*.h files will not be generated
add_executable(${app} WIN32
    ${GUI_HDR}
    ${GUI_SRC}
    ${GUI_MOC_SRC}
    ${GUI_UIS_H}
    ${GUI_RC_CXX}
)

target_link_libraries(${app} roboime-intel)

qt5_use_modules(${app} Core Widgets OpenGL Gui)

set_property(TARGET ${app} PROPERTY FOLDER apps)

