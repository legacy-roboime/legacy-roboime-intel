cmake_minimum_required(VERSION 2.8.8)

project(RoboIME)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake/modules)
include(${PROJECT_SOURCE_DIR}/../cmake/Utils.cmake)

standard_config()
standard_paths(${PROJECT_SOURCE_DIR}/.. bin lib)

include_directories(.)

add_subdirectory(communication)
add_subdirectory(3rdparty/samplescommoncode)
add_subdirectory(3rdparty/nxustream2)
add_subdirectory(simulation)
add_subdirectory(soccer)
add_subdirectory(intelligence)
add_subdirectory(clients/simulation)
add_subdirectory(clients/logplayer)
add_subdirectory(clients/intelligence)
add_subdirectory(clients/soccer)
# add new projects here
