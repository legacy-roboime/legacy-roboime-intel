cmake_minimum_required(VERSION 2.8)


# Include all headers
include_directories(.)
file(GLOB NXU_HDR "*.h")

aux_source_directory(. NXU_SRC)

# adding to group
source_group("Headers" FILES ${NXU_HDR})
source_group("Sources" FILES ${NXU_SRC})


##############################
# PhysX stuff
#

find_package(PhysX REQUIRED)
include_directories(${PHYSX_INCLUDES})


##############################
# Le finale
#

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