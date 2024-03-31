set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LibError_HeaderFiles ${LibError_HeaderFiles}
    "${DIR}/DefaultError.hpp"
    "${DIR}/TraceError.hpp"

    PARENT_SCOPE
)
