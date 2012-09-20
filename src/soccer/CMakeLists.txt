cmake_minimum_required(VERSION 2.8)

##############################
# Include all headers
#

include_directories(.)
file(GLOB SOC_HDR "*.h")

##############################
# Files which will be compiled
#

set(SOC_SRC
    vector.c
    soccer.c
    soccer_action.c
    soccer_blue.c
    soccer_red.c
    soccer_env.c
    soccer_draw.c
    minimax.c
    move_table.c
)



##############################
# Dependencies
#

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})

if(WIN32)
    set(SOC_LIBS ${GLUT_LIBRARIES} ${OPENGL_LIBRARY})
else()
    set(SOC_LIBS ${GLUT_LIBRARIEs} ${OPENGL_LIBRARY} m)
endif()



##############################
# Le finale
#

# adding to group
directory_source_group(headers . *.h)
directory_source_group(src . *.c)

set(SOC_INC_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    PARENT_SCOPE
)

add_library(soccer
    ${SOC_SRC}
    ${SOC_HDR}
)

target_link_libraries(soccer
    ${SOC_LIBS}
)

set_property(TARGET soccer PROPERTY FOLDER modules)

