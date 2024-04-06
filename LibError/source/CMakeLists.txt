add_subdirectory(types)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LibError_SourceFiles ${LibError_SourceFiles}
    "${DIR}/LibError.cppm"
    "${DIR}/Stl.cppm"

    PARENT_SCOPE
)
