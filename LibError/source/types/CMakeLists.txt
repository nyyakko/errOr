set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LibError_SourceFiles ${LibError_SourceFiles}
    "${DIR}/Types.cppm"
    "${DIR}/StandardError.cppm"
    "${DIR}/TraceError.cppm"

    PARENT_SCOPE
)
