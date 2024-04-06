cmake_minimum_required(VERSION 3.28)

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
# set(LibError_CompilerOptions ${LibError_CompilerOptions})

# set(LibError_ExternalLibraries)

add_subdirectory(LibError)
