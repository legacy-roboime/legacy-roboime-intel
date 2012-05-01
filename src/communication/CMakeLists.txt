cmake_minimum_required(VERSION 2.8)

##############################
# Files which need to be moc'd
# Ideally this should be automatic, but it isn't yet
#

set(COMM_MOC
    UdpServer.h
    # add new headers here
)

##############################
# Include all headers
#

include_directories(.)
file(GLOB COMM_HDR "*.h")

##############################
# Files which will be compiled
#

set(COMM_SRC
    UdpServer.cpp
    # add new sources here
)



##############################
# Protobuf files
set(COMM_PROTO
    messages_internal_command.proto
    messages_robocup_ssl_detection.proto
    messages_robocup_ssl_geometry.proto
    messages_robocup_ssl_refbox_log.proto
    messages_robocup_ssl_wrapper.proto
    # add new protos here
)

# adding to group
source_group("Headers" FILES ${COMM_HDR})
source_group("Sources" FILES ${COMM_SRC})



##############################
# Qt stuff
#

find_package(Qt4 REQUIRED)
set(QT_DONT_USE_QTGUI true)
set(QT_USE_QTNETWORK true)
include(${QT_USE_FILE})
qt4_wrap_cpp(COMM_MOC_CPP ${COMM_HDR})



##############################
# Protobuf stuff
#

find_package(Protobuf REQUIRED)
protobuf_generate_cpp(COMM_PROTO_SRC COMM_PROTO_HDR ${COMM_PROTO})

include_directories(${PROTOBUF_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

# adding to group
source_group("Messages" FILES ${COMM_PROTO})



##############################
# Le finale
#

set(COMM_GEN
    ${COMM_MOC_CPP}
    ${COMM_PROTO_SRC}
    ${COMM_PROTO_HDR}
)

set(COMM_INC_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROTOBUF_INCLUDE_DIRS}
    PARENT_SCOPE
)

# adding to group
source_group("Generated" FILES ${COMM_GEN})

add_library(communication
    ${COMM_GEN}
    ${COMM_HDR}
    ${COMM_SRC}
    ${COMM_PROTO}
)

target_link_libraries(communication
    ${PROTOBUF_LIBRARIES}
)
