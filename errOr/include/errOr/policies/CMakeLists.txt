set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(errOr_HeaderFiles ${errOr_HeaderFiles}
    "${DIR}/DefaultError.hpp"
    "${DIR}/TraceableError.hpp"

    PARENT_SCOPE
)
