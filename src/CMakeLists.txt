cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake)

project(RoboIME)


add_subdirectory(communication)
add_subdirectory(utils/samplescommoncode)
add_subdirectory(utils/nxustream2)
add_subdirectory(simulation)
add_subdirectory(soccer)
add_subdirectory(intelligence)
add_subdirectory(clients/simulation)
add_subdirectory(clients/logplayer)
add_subdirectory(clients/intelligence)
# add new projects here
