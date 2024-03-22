set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(err_or_HeaderFiles ${err_or_HeaderFiles}
    "${DIR}/DefaultError.hpp"
    "${DIR}/TraceError.hpp"

    PARENT_SCOPE
)
