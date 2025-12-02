# ============================================================================
# MaratonaScore Legacy CLI
# ============================================================================
# Original simple CLI with positional arguments.
# DEPRECATED: Use MaratonaScoreCLI instead.
# This executable will be removed in v4.0.0
# ============================================================================

# Source files
set(LEGACY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# Create executable
add_executable(maratona_score ${LEGACY_SOURCES})

# Set executable properties
set_target_properties(maratona_score PROPERTIES
    OUTPUT_NAME "maratona_score"
)

# Link dependencies
target_link_libraries(maratona_score
    PRIVATE
        MaratonaScoreLib
)

# Add deprecation warning definition
target_compile_definitions(maratona_score
    PRIVATE
        DEPRECATED_CLI
        LEGACY_VERSION="2.0.0"
        NEW_VERSION="3.0.0"
)

# Compiler warnings
if(MSVC)
    target_compile_options(maratona_score PRIVATE /W4)
else()
    target_compile_options(maratona_score PRIVATE -Wall -Wextra -pedantic)
endif()
