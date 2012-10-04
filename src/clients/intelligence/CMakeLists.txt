cmake_minimum_required(VERSION 2.8)

##############################
# Include all headers
#

include_directories(.)
file(GLOB INTC_HDR "*.h")

##############################
# Files which will be compiled
#

set(INTC_SRC
    main.cpp
    Intelligence.cpp
    # add new sources here
)



##############################
# Intra dependencies
#

# Intelligence
include_directories(${INT_INC_DIRS})



##############################
# Le finale
#

# adding to group
directory_source_group(headers . *.h)
directory_source_group(src . *.cpp)
source_group(generated REGULAR_EXPRESSION ".*_automoc[.]cpp")

add_executable(intelligence-client
    ${INTC_HDR}
    ${INTC_SRC}
)

target_link_libraries(intelligence-client
    intelligence
    ${GLUT_LIBRARIES}
    ${QT_LIBRARIES}
)

set_property(TARGET intelligence-client PROPERTY FOLDER apps)

