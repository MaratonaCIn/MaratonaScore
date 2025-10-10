# Core library - Business logic (GUI-independent)

# Collect all source files
set(CORE_SOURCES
    services/ScoreCalculator.cpp
    services/ContestProcessor.cpp
    services/RatingSystem.cpp
    persistence/DatabaseManager.cpp
)

# Create static library
add_library(MaratonaScore_core STATIC ${CORE_SOURCES})

# Link dependencies
target_link_libraries(MaratonaScore_core
    PUBLIC
        nlohmann_json::nlohmann_json
)

# Include directories
target_include_directories(MaratonaScore_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/MaratonaScore
)

# Set compile options
target_compile_options(MaratonaScore_core
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)
