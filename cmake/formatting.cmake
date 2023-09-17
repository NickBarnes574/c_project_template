# create string variable called CMAKE_CHECKS
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

# setup clang tidy
set(CMAKE_C_CLANG_TIDY
    "clang-tidy" "-checks=${CMAKE_CHECKS}" "-warnings-as-errors=*"
)