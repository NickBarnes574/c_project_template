cmake_minimum_required(VERSION 3.16)

project(
    NewProject
    LANGUAGES "C"
    DESCRIPTION "New Project v1.0"
)

# Clang tidy setup
find_program(CLANG_TIDY_PROG clang-tidy)
if(CLANG_TIDY_PROG)
    set(CMAKE_C_CLANG_TIDY 
        "${CLANG_TIDY_PROG};--checks=*,-llvm-include-order,-cppcoreguidelines-*,\
        -readability-magic-numbers,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling,\
        -hiccp-no-assembler")
else()
    set(CMAKE_C_COMPILER gcc)
endif()

# Function to setup target
function(setup_target TARGET_NAME SOURCE_DIR)
    target_include_directories(${TARGET_NAME} PRIVATE ${SOURCE_DIR}/include)
    if(CLANG_TIDY_PROG)
        set_target_properties(${TARGET_NAME} PROPERTIES C_CLANG_TIDY "${CMAKE_C_CLANG_TIDY}")
        target_compile_options(${TARGET_NAME} PRIVATE -g)
    else()
        target_compile_options(${TARGET_NAME} PRIVATE -g -Wall -pedantic)
    endif()
endfunction()


include(new_project_main.cmake)

include_directories(0_Common/include)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/0_Common/CMakeLists.txt")
    add_subdirectory(0_Common)
endif()

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/1_Math/CMakeLists.txt")
    add_subdirectory(1_Math)
endif()

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/2_DataStructures/CMakeLists.txt")
    add_subdirectory(2_DataStructures)
endif()

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/3_Threading/CMakeLists.txt")
    add_subdirectory(3_Threading)
endif()

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/4_Networking/CMakeLists.txt")
    add_subdirectory(4_Networking)
endif()

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/5_NewProject/CMakeLists.txt")
    add_subdirectory(5_NewProject)
endif()

# EOF
