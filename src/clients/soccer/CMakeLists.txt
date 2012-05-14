cmake_minimum_required(VERSION 2.8)

##############################
# Files which need to be moc'd
#

# for convenience we moc EVERYTHING
# that's not perfect, but it's not that bad either
file(GLOB_RECURSE SOCC_MOC "*.h")

##############################
# Include all headers
#

include_directories(.)
file(GLOB SOCC_HDR "*.h")
source_group("Headers" FILES ${SOCC_HDR})

##############################
# Files which will be compiled
#

set(SOCC_SRC
    main3.c
    # add new sources here
)
source_group("Sources" FILES ${SOCC_SRC})



##############################
# Intra dependencies
#

# Soccer
include_directories(${SOC_INC_DIRS})



##############################
# Qt stuff
#

find_package(Qt4 REQUIRED)
set(QT_USE_QTNETWORK true)
set(QT_USE_QTOPENGL true)
include(${QT_USE_FILE})
qt4_wrap_cpp(SOCC_MOC_CPP ${SOCC_MOC})



##############################
# Le finale
#

set(GEN_FILES
    ${SOCC_MOC_CPP}
)

# adding to group
source_group("Generated" FILES ${GEN_FILES})

add_executable(soccer-client
    ${GEN_FILES}
    ${SOCC_HDR}
    ${SOCC_SRC}
)

target_link_libraries(soccer-client
    soccer
    ${QT_LIBRARIES}
)
