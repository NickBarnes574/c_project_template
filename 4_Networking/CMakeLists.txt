# Minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

# Project Name
project(Networking LANGUAGES C)

# Library Sources
set(LIBRARY_SOURCES
    src/tcp_server.c
    src/socket_io.c
    )

# Create the Networking library
add_library(Networking ${LIBRARY_SOURCES})
setup_target(Networking ${Networking_SOURCE_DIR})

# Specify the C standard
set_property(TARGET Networking PROPERTY C_STANDARD 11)

# Include directories
target_include_directories(Networking PUBLIC include/)

# Link against Common and Threading libraries
target_link_libraries(Networking PUBLIC Common Threading)

# Include directories for Common library
target_include_directories(Networking PUBLIC
    $<TARGET_PROPERTY:Common,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:Threading,INTERFACE_INCLUDE_DIRECTORIES>
)
