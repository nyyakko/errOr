function(enable_tests)

    message(STATUS "[${PROJECT_NAME}] project will be building the test suit.")

    if (NOT ${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        message(STATUS "[${PROJECT_NAME}] running on ${CMAKE_HOST_SYSTEM_NAME}, sanitizers for tests are enabled.")
        set(err_or_TestsLinkerOptions ${err_or_TestsLinkerOptions} -fsanitize=undefined,leak,address)
    else()
        message(STATUS "[${PROJECT_NAME}] running on ${CMAKE_HOST_SYSTEM_NAME}, sanitizers for tests are disabled.")
    endif()

    set(err_or_TestsCompilerOptions ${err_or_TestsCompilerOptions} ${err_or_CompilerOptions})

    include(GoogleTest)
    find_package(GTest CONFIG REQUIRED)

    enable_testing()
    add_subdirectory(tests)

endfunction()
