# Minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

# Project Name
project(Threading LANGUAGES C)

# Library Sources
set(LIBRARY_SOURCES
    src/threadpool.c
    )

# Create the Threading library
add_library(Threading ${LIBRARY_SOURCES})
setup_target(Threading ${Threading_SOURCE_DIR})

# Specify the C standard
set_property(TARGET Threading PROPERTY C_STANDARD 11)

# Include directories
target_include_directories(Threading PUBLIC include/)

# Link against Common and DataStructures libraries
target_link_libraries(Threading PUBLIC Common DataStructures pthread)

# Include directories for Common and DataStructures
target_include_directories(Threading PUBLIC
    ${CMAKE_SOURCE_DIR}/0_Common/include
    ${CMAKE_SOURCE_DIR}/2_DataStructures/include
)

