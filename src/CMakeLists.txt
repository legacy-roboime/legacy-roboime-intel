cmake_minimum_required(VERSION 2.8.9)

project(RoboIME)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake/modules)
include(${PROJECT_SOURCE_DIR}/../cmake/Utils.cmake)

standard_config()
standard_paths(${PROJECT_SOURCE_DIR}/.. bin lib)
message(STATUS "Project src dir: ${PROJECT_SOURCE_DIR}")

include_directories(.)

if(MSVC)
    add_definitions(-DHAVE_WINDOWS)
endif()

if(UNIX)
    add_definitions(-DHAVE_UNIX)
    if(APPLE)
        add_definitions(-DHAVE_MACOSX)
    else()
        #TODO: fix this, say we have FreeBSD, that's not linux
        add_definitions(-DHAVE_LINUX)
    endif()
endif()

# Qt stuff
find_package(Qt5Core)

#add_definitions(-DUSE_PHYSX)
#add_definitions(-DUSE_SIMULATION)

# Protobuf stuff
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

add_subdirectory(intelligence)
add_subdirectory(clients/intelligence)
add_subdirectory(clients/graphicalintelligence)
# add new projects here

