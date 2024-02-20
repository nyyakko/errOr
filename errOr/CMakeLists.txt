add_subdirectory(include/${PROJECT_NAME})

add_library(${PROJECT_NAME} INTERFACE ${errOr_HeaderFiles})

if (ENABLE_CLANGTIDY)
    enable_clang_tidy()
endif()

if (ENABLE_CPPCHECK)
    enable_cppcheck()
endif()

target_include_directories(${PROJECT_NAME}
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERIFY_INTERFACE_HEADER_SETS TRUE)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_23)

target_sources(${PROJECT_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS include FILES ${errOr_HeaderFiles})

export(TARGETS      ${PROJECT_NAME}
       FILE         ${EXPORT_DIR}/${PROJECT_NAME}Targets.cmake
       NAMESPACE    ${PROJECT_NAME}::
)

install(TARGETS     ${PROJECT_NAME}
        EXPORT      ${PROJECT_NAME}Targets
        ARCHIVE
        FILE_SET HEADERS
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT      ${PROJECT_NAME}Targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/cmake
        NAMESPACE   ${PROJECT_NAME}::
)

install(FILES ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/cmake)

target_compile_options(${PROJECT_NAME} INTERFACE ${errOr_CompilerOptions})
target_link_libraries(${PROJECT_NAME} INTERFACE ${errOr_ExternalLibraries})
