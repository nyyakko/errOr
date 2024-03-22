cmake_minimum_required(VERSION 3.25)

project(err_or LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(EXPORT_DIR "${CMAKE_SOURCE_DIR}/build/cmake")

include(static_analyzers)
include(enable_vcpkg)
include(enable_tests)
include(GNUInstallDirs)

option(ENABLE_CLANGTIDY "" TRUE)
option(ENABLE_CPPCHECK "" TRUE)
option(ENABLE_TESTING "" TRUE)

if (ENABLE_TESTING)
    enable_tests()
endif()

enable_vcpkg()

# set(err_or_LinkerOptions ${err_or_LinkerOptions})
# set(err_or_CompilerOptions ${err_or_CompilerOptions})

# find_package(package1 CONFIG REQUIRED)
# find_package(package2 CONFIG REQUIRED)
# ...
#
# set(err_or_ExternalLibraries package1 package2 ...)

add_subdirectory(err_or)
