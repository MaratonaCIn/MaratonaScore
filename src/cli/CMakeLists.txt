# CLI library - Command-line interface

# Collect all source files
set(CLI_SOURCES
    CLIHandler.cpp
    OutputFormatter.cpp
    ExcelConverter.cpp
)

# Create static library
add_library(MaratonaScore_cli STATIC ${CLI_SOURCES})

# Link dependencies
target_link_libraries(MaratonaScore_cli
    PUBLIC
        MaratonaScore_core
        nlohmann_json::nlohmann_json
)

# Include directories
target_include_directories(MaratonaScore_cli
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/MaratonaScore
)

# Set compile options
target_compile_options(MaratonaScore_cli
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)
