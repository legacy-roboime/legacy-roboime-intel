cmake_minimum_required(VERSION 2.8.8)

# Include all headers
include_directories(.)
file(GLOB SCC_HDR "*.h")

aux_source_directory(. SCC_SRC)
if(WIN32)
    include_directories(win)
    file(GLOB SCC_HDR_X "win/*.h")
    aux_source_directory(win SCC_SRC_X)
    set(EXTRA_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/win)
endif()



##############################
# Le finale
#

# adding to group
directory_source_group(headers . *.h)
directory_source_group(src . *.cpp)

set(SCC_INC_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${EXTRA_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    ${PHYSX_INCLUDES}
    PARENT_SCOPE
)

add_library(samplescommoncode
    ${SCC_HDR}
    ${SCC_SRC}
    ${SCC_HDR_X}
    ${SCC_SRC_X}
)

target_link_libraries(samplescommoncode
    ${PHYSX_LIBRARIES}
    ${GLUT_LIBRARIES}
)

set_property(TARGET samplescommoncode PROPERTY FOLDER 3rdparty)

