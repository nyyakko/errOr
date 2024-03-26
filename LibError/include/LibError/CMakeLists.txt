add_subdirectory(types)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LibError_HeaderFiles ${LibError_HeaderFiles}
    "${DIR}/ErrorOr.hpp"

    PARENT_SCOPE
)
