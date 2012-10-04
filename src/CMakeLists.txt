cmake_minimum_required(VERSION 2.8)

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
set(QT_USE_QTNETWORK true)
set(QT_USE_QTOPENGL true)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

# PhysX stuff
find_package(PhysX)
if(PHYSX_FOUND)
    include_directories(${PHYSX_INCLUDES})
    add_definitions(-DUSE_PHYSX)
    add_definitions(-DUSE_SIMULATION)
else()
    # mockup to prevent error on cmake
    #FIXME: this is ugly
    set(PHYSX_INCLUDES)
    set(PHYSX_LIBRARIES)
endif()

# Protobuf stuff
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

# Kalman
find_library(KALMAN_LIBRARY "libKalman.lib" "${PROJECT_SOURCE_DIR}/../lib")
find_library(KALMAN_LIBRARY_DEBUG "libKalmand.lib" "${PROJECT_SOURCE_DIR}/../lib")
if(NOT KALMAN_LIBRARY_FOUND)
    # mockup to prevent error on cmake
    #FIXME: this is ugly
    set(KALMAN_LIBRARY)
endif()

# GLUT stuff
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

add_subdirectory(communication)
if(PHYSX_FOUND)
    add_subdirectory(3rdparty/samplescommoncode)
    add_subdirectory(3rdparty/nxustream2)
    add_subdirectory(simulation)
    add_subdirectory(clients/simulation)
    add_subdirectory(clients/logplayer)
endif()
add_subdirectory(soccer)
add_subdirectory(intelligence)
add_subdirectory(clients/intelligence)
add_subdirectory(clients/graphicalintelligence)
add_subdirectory(clients/soccer)
# add new projects here

