# ============================================================================
# MaratonaScore CLI
# ============================================================================
# Modern command-line interface with subcommands:
#   - process:  Process contests and generate scoreboards
#   - inspect:  Analyze contest data
#   - config:   Manage configuration
#   - init:     Initialize project structure
#   - validate: Validate input files
#   - export:   Export cached results
# ============================================================================

# Collect all source files
file(GLOB_RECURSE CLI_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Create executable
add_executable(MaratonaScoreCLI ${CLI_SOURCES})

# Set executable properties
set_target_properties(MaratonaScoreCLI PROPERTIES
    OUTPUT_NAME "maratona_score_cli"
)

# Include directories
target_include_directories(MaratonaScoreCLI
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(MaratonaScoreCLI
    PRIVATE
        MaratonaScoreLib
        CLI11::CLI11
        # nlohmann_json::nlohmann_json
)

# Compiler warnings
if(MSVC)
    target_compile_options(MaratonaScoreCLI PRIVATE /W4)
else()
    target_compile_options(MaratonaScoreCLI PRIVATE -Wall -Wextra -pedantic)
endif()

# Enable colored output on Windows
if(WIN32)
    target_compile_definitions(MaratonaScoreCLI PRIVATE
        _CRT_SECURE_NO_WARNINGS
        NOMINMAX
    )
endif()
