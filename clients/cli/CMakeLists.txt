cmake_minimum_required(VERSION 2.8.9)

project(roboime-cli)

##############################
# Source files
#

set(src src)
include_directories(${src})

set(CLI_HDR
    ${src}/Intelligence.h
)

set(CLI_SRC
    ${src}/main.cpp
    ${src}/Intelligence.cpp
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
directory_source_group(headers ${src} *.h)
directory_source_group(headers ${src} *.hh)
directory_source_group(headers ${src} *.hpp)
directory_source_group(src ${src} *.cc)
directory_source_group(src ${src} *.cxx)
directory_source_group(src ${src} *.cpp)
source_group(generated REGULAR_EXPRESSION ".*_automoc[.]cpp")

set(app roboime-cli)

qt5_wrap_cpp(CLI_MOC_SRC ${CLI_HDR} OPTIONS -nn)

add_executable(${app}
    ${CLI_HDR}
    ${CLI_SRC}
    ${CLI_MOC_SRC}
)

target_link_libraries(${app} roboime-intel)

qt5_use_modules(${app} Core Widgets)

set_property(TARGET ${app} PROPERTY FOLDER apps)

