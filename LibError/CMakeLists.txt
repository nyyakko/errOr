add_subdirectory(source)
add_subdirectory(include/liberror)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if (ENABLE_CLANGTIDY)
    # enable_clang_tidy()
endif()

if (ENABLE_CPPCHECK)
    enable_cppcheck()
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/liberror"
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET CXX_MODULES BASE_DIRS source
            FILES ${LibError_SourceFiles}
            FILE_SET HEADERS BASE_DIRS include
            FILES ${LibError_HeaderFiles}
)

export(TARGETS      ${PROJECT_NAME}
       FILE         ${EXPORT_DIR}/${PROJECT_NAME}Targets.cmake
       NAMESPACE    ${PROJECT_NAME}::
)

install(TARGETS     ${PROJECT_NAME}
        EXPORT      ${PROJECT_NAME}Targets
        ARCHIVE
        FILE_SET HEADERS
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILE_SET CXX_MODULES
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}
)

install(EXPORT      ${PROJECT_NAME}Targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/cmake
        NAMESPACE   ${PROJECT_NAME}::
)

install(FILES ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/cmake)

target_link_options(${PROJECT_NAME} PRIVATE ${LibError_LinkerOptions})
target_compile_options(${PROJECT_NAME} PRIVATE ${LibError_CompilerOptions})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LibError_ExternalLibraries})
