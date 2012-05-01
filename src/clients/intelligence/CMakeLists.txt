cmake_minimum_required(VERSION 2.8)

##############################
# Files which need to be moc'd
#

# for convenience we moc EVERYTHING
# that's not perfect, but it's not that bad either
file(GLOB_RECURSE INTC_MOC "*.h")

##############################
# Include all headers
#

include_directories(.)
file(GLOB INTC_HDR "*.h")
source_group("Headers" FILES ${INTC_HDR})

##############################
# Files which will be compiled
#

set(INTC_SRC
    main.cpp
    Intelligence.cpp
    # add new sources here
)
source_group("Sources" FILES ${INTC_SRC})



##############################
# Intra dependencies
#

# Intelligence
include_directories(${INT_INC_DIRS})



##############################
# Qt stuff
#

find_package(Qt4 REQUIRED)
set(QT_USE_QTNETWORK true)
set(QT_USE_QTOPENGL true)
include(${QT_USE_FILE})
qt4_wrap_cpp(INTC_MOC_CPP ${INTC_MOC})



##############################
# Le finale
#

set(GEN_FILES
    ${INTC_MOC_CPP}
)

# adding to group
source_group("Generated" FILES ${GEN_FILES})

add_executable(intelligence-client
    ${GEN_FILES}
    ${INTC_HDR}
    ${INTC_SRC}
)

target_link_libraries(intelligence-client
    intelligence
    ${QT_LIBRARIES}
)
