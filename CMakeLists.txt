cmake_minimum_required(VERSION 3.25)

project(LibError LANGUAGES CXX)

include(cmake/static_analyzers.cmake)
include(cmake/get_cpm.cmake)
include(cmake/enable_tests.cmake)
include(GNUInstallDirs)

if (ENABLE_TESTING)
    enable_tests(${PROJECT_NAME})
endif()

# set(LibError_CompilerOptions ${LibError_CompilerOptions})
# set(LibError_LinkerOptions ${LibError_LinkerOptions})

CPMAddPackage("gh:fmtlib/fmt#10.2.1")
CPMAddPackage("gh:nyyakko/expected#35de9a0")

set(LibError_ExternalLibraries fmt::fmt expected::expected)

add_subdirectory(LibError)
