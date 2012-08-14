cmake_minimum_required(VERSION 2.8)

##############################
# Files which will be compiled
#

set(SOCC_SRC
    main3.c
    # add new sources here
)

# Include soccer
include_directories(${SOC_INC_DIRS})



##############################
# Le finale
#

# adding to group
directory_source_group(src . *.cpp)

add_executable(soccer-client ${SOCC_SRC})
target_link_libraries(soccer-client soccer)

set_property(TARGET soccer-client PROPERTY FOLDER apps)

