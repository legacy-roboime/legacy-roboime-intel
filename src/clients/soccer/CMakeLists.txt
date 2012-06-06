cmake_minimum_required(VERSION 2.8)

##############################
# Files which will be compiled
#

set(SOCC_SRC
    main3.c
    # add new sources here
)
source_group("Sources" FILES ${SOCC_SRC})

# Include soccer
include_directories(${SOC_INC_DIRS})



##############################
# Le finale
#

add_executable(soccer-client ${SOCC_SRC})
target_link_libraries(soccer-client soccer)
