cmake_minimum_required(VERSION 2.8)

# Include all headers
include_directories(.)
file(GLOB NXU_HDR "*.h")

aux_source_directory(. NXU_SRC)



##############################
# Le finale
#

# adding to group
directory_source_group(headers . *.h)
directory_source_group(src . *.cpp)

set(NXU_INC_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PHYSX_INCLUDES}
    PARENT_SCOPE
)

add_library(nxustream2
    ${NXU_HDR}
    ${NXU_SRC}
)

target_link_libraries(nxustream2
    ${PHYSX_LIBRARIES}
)

set_property(TARGET nxustream2 PROPERTY FOLDER 3rdparty)

