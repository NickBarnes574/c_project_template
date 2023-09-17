# Minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

# Project Name
project(NewProject LANGUAGES C)

# Project Sources
set(SOURCES
    src/new_project_main.c
    )

add_executable(NewProject ${SOURCES})

# Link against Common and Networking libraries
target_link_libraries(NewProject PUBLIC Common Networking Threading)
