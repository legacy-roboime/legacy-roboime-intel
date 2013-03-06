cmake_minimum_required(VERSION 2.8.9)

project(roboime-cli)

##############################
# Include all headers
#

set(src src)
include_directories(${src})
file(GLOB CLI_HDR RELATIVE ${src} "*.h")

##############################
# Files which will be compiled
#

set(CLI_SRC
    ${src}/main.cpp
    ${src}/Intelligence.cpp
    # add new sources here
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
directory_source_group(headers . *.h)
directory_source_group(src . *.cpp)
source_group(generated REGULAR_EXPRESSION ".*_automoc[.]cpp")

set(app roboime-cli)

add_executable(${app}
    ${CLI_HDR}
    ${CLI_SRC}
)

target_link_libraries(${app} roboime-intel)

qt5_use_modules(${app} Core Widgets)

set_property(TARGET ${app} PROPERTY FOLDER apps)

