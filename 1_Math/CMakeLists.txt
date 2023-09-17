# Minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

# Project Name
project(Math LANGUAGES C)

# Library Sources
set(LIBRARY_SOURCES
    src/math_functions_32_bit.c
    src/math_functions_64_bit.c
    src/number_converter.c
    )

# Create the Math library
add_library(Math ${LIBRARY_SOURCES})
setup_target(Math ${Math_SOURCE_DIR})

# Specify the C standard
set_property(TARGET Math PROPERTY C_STANDARD 11)

# Include directories
target_include_directories(Math PUBLIC include/)

# Link against Common library
target_link_libraries(Math PUBLIC Common)

# Include directory for Common
target_include_directories(Math PUBLIC
    $<TARGET_PROPERTY:Common,INTERFACE_INCLUDE_DIRECTORIES>
)