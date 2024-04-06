set(DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TEST_NAME runtime)

project(${TEST_NAME} LANGUAGES CXX)

add_executable(${TEST_NAME} "${DIR}/${TEST_NAME}.cpp")

# enable_clang_tidy()
# enable_cppcheck()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_link_libraries(${TEST_NAME} PRIVATE gtest_main gtest LibError::LibError)
target_compile_options(${TEST_NAME} PRIVATE ${LibError_TestsCompilerOptions})
target_link_options(${TEST_NAME} PRIVATE ${LibError_TestsLinkerOptions})

gtest_discover_tests(${TEST_NAME})
