cmake_minimum_required(VERSION 2.8)

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
#

set(COMM_PROTO
    messages_internal_command.proto
    messages_robocup_ssl_detection.proto
    messages_robocup_ssl_geometry.proto
    messages_robocup_ssl_refbox_log.proto
    messages_robocup_ssl_wrapper.proto
    grSim_Commands.proto
    grSim_Replacement.proto
    grSim_Packet.proto
    # add new protos here
)

protobuf_generate_cpp(COMM_PROTO_SRC COMM_PROTO_HDR ${COMM_PROTO})
include_directories(${CMAKE_CURRENT_BINARY_DIR})



##############################
# Le finale
#

set(COMM_GEN
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
source_group("generated" FILES ${COMM_GEN})
directory_source_group(headers . *.h)
directory_source_group(src . *.cpp)
directory_source_group(proto . *.proto)
source_group(generated REGULAR_EXPRESSION ".*_automoc[.]cpp")

add_library(communication
    ${COMM_GEN}
    ${COMM_HDR}
    ${COMM_SRC}
    ${COMM_PROTO}
)

target_link_libraries(communication
    ${PROTOBUF_LIBRARIES}
)

set_property(TARGET communication PROPERTY FOLDER modules)

