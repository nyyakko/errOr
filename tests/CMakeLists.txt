set(LibError_TestsCompilerOptions ${LibError_TestsCompilerOptions}
    -Wno-gnu-statement-expression-from-macro-expansion
)

add_subdirectory(compiletime)
add_subdirectory(initialization)
add_subdirectory(runtime)

