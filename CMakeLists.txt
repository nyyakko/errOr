cmake_minimum_required(VERSION 3.25)

project(LibError LANGUAGES CXX)

set(EXPORT_DIR "${CMAKE_SOURCE_DIR}/build/cmake")

include(cmake/CPM.cmake)
include(cmake/static_analyzers.cmake)
include(cmake/enable_tests.cmake)
include(GNUInstallDirs)

option(ENABLE_CLANGTIDY "" TRUE)
option(ENABLE_CPPCHECK "" TRUE)
option(ENABLE_TESTING "" TRUE)

if (ENABLE_TESTING)
    enable_tests()
endif()

# set(LibError_LinkerOptions ${LibError_LinkerOptions})
set(LibError_CompilerOptions ${LibError_CompilerOptions} -Wno-language-extension-token)

CPMAddPackage(
    NAME googletest
    GITHUB_REPOSITORY google/googletest
    GIT_TAG release-1.12.1
    VERSION 1.12.1
    OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
)

set(LibError_ExternalLibraries)

add_subdirectory(LibError)
