cmake_minimum_required(VERSION 2.8)

project(RoboIME)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake/modules)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
endforeach()
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_MINSIZEREL_POSTFIX min)

include_directories(.)

add_subdirectory(communication)
add_subdirectory(utils/samplescommoncode)
add_subdirectory(utils/nxustream2)
add_subdirectory(simulation)
add_subdirectory(soccer)
add_subdirectory(intelligence)
add_subdirectory(clients/simulation)
add_subdirectory(clients/logplayer)
add_subdirectory(clients/intelligence)
add_subdirectory(clients/soccer)
# add new projects here
