cmake_minimum_required(VERSION 3.16)
project(DataStructures VERSION 0.0.0 LANGUAGES "C")

# Set the C standard
set(CMAKE_C_STANDARD 11)

# DataStructures target
set(LIBRARY_SOURCES
    src/queue.c
    # add more data structure source files here as they are created
)

add_library(DataStructures SHARED ${LIBRARY_SOURCES})
setup_target(DataStructures ${DataStructures_SOURCE_DIR})

# Tests
if(EXISTS ${DataStructures_SOURCE_DIR}/tests/queue_tests.c)
    add_executable(test_queue ${DataStructures_SOURCE_DIR}/tests/queue_tests.c)
    setup_target(test_queue ${DataStructures_SOURCE_DIR})
    target_link_libraries(test_queue DataStructures cunit Common)
endif()
