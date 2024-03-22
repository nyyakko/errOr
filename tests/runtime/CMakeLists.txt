set(DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TEST_NAME runtime)

project(${TEST_NAME} LANGUAGES CXX)

add_executable(${TEST_NAME} "${DIR}/${TEST_NAME}.cpp")

enable_clang_tidy()
enable_cppcheck()

target_link_libraries(${TEST_NAME} PRIVATE GTest::gtest_main err_or)
target_compile_options(${TEST_NAME} PRIVATE
    $<$<NOT:$<STREQUAL:${CMAKE_CXX_COMPILER_ID},MSVC>>:-Wno-gnu-statement-expression-from-macro-expansion>
)
# target_link_options(${TEST_NAME} PRIVATE ${err_or_TestsLinkerOptions})

gtest_discover_tests(${TEST_NAME})
