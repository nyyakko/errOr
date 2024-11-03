add_subdirectory(types)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LibError_HeaderFiles ${LibError_HeaderFiles}
    "${DIR}/Maybe.hpp"

    PARENT_SCOPE
)
