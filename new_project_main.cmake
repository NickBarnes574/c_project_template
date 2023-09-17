string(CONCAT CMAKE_CHECKS
    "*,-readability-function-cognitive-complexity,"
    "-altera*,"
    "-cert-dcl03-c,"
    "-misc-static-assert,"
    "-llvm-include-order,"
    "-llvmlibc-*,"
    "-hicpp-*,"
    "-concurrency-mt-unsafe,"
    "-bugprone-easily-swappable-parameters,"
    "-cppcoreguidelines-*,"
    "-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling"
)

if(CMAKE_BUILD_TYPE MATCHES debug)
    message(STATUS "*** building in DEBUG mode ***")

    add_compile_options(-std=c17 -Wall -Wextra -pedantic -Werror
        -fsanitize=address -g)
    link_libraries(-fsanitize=address)
    add_compile_definitions(DEBUG)
    set(CMAKE_C_CLANG_TIDY
        "clang-tidy" "-checks=${CMAKE_CHECKS}" "-warnings-as-errors=*"
    )
elseif(CMAKE_BUILD_TYPE MATCHES test)
    message(STATUS "*** building in TEST mode ***")
    enable_testing()
    set(CTEST_RERUN_FAILED ON)
    set(CTEST_OUTPUT_ON_FAILURE ON)
    add_compile_options(-std=c17 -Wall -Wextra -pedantic -g)
else()
    message(STATUS "*** building in RELEASE mode ***")
    add_compile_options(-std=c17 -Wall -Wextra -pedantic)
    add_compile_definitions(NDEBUG)
    set(CMAKE_C_CLANG_TIDY
        "clang-tidy" "-checks=${CMAKE_CHECKS}"
    )
endif()

# EOF