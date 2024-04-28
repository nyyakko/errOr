cmake_minimum_required(VERSION 3.25)

project(LibError LANGUAGES CXX)

set(EXPORT_DIR "${CMAKE_SOURCE_DIR}/build/cmake")

include(cmake/CPM.cmake)
include(cmake/static_analyzers.cmake)
include(cmake/enable_tests.cmake)
include(GNUInstallDirs)

if (ENABLE_TESTING)
    enable_tests(${PROJECT_NAME})
endif()

# set(LibError_CompilerOptions ${LibError_CompilerOptions})
# set(LibError_LinkerOptions ${LibError_LinkerOptions})

CPMAddPackage("gh:fmtlib/fmt#10.2.1")
CPMAddPackage("gh:nyyakko/expected@master#bdc55a0")

set(LibError_ExternalLibraries fmt::fmt expected::expected)

add_subdirectory(LibError)
