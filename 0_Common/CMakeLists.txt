# Minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

# Project Name
project(Common LANGUAGES C)

# Library Sources
set(LIBRARY_SOURCES
    src/utilities.c
    src/file_io.c
    src/comparisons.c
    src/signal_handler.c
    )

# Create the Common library
add_library(Common ${LIBRARY_SOURCES})
setup_target(Common ${Common_SOURCE_DIR})

# Specify the C standard
set_property(TARGET Common PROPERTY C_STANDARD 11)

# Include directories
target_include_directories(Common PUBLIC include/)
