# ============================================================================
# MaratonaScore Library
# ============================================================================
# Core shared library containing all business logic:
#   - Models (Contest, Contestant, Performance, Scoreboard)
#   - Parsers (vJudge Excel, Finals text)
#   - Scoring algorithms
#   - Utilities (Settings, Blacklist)
# ============================================================================

# Collect all source files
file(GLOB_RECURSE LIB_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Create shared library
add_library(MaratonaScoreLib SHARED ${LIB_SOURCES})

# Set library properties
set_target_properties(MaratonaScoreLib PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME "maratona_score"
)

# Include directories
target_include_directories(MaratonaScoreLib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/maratona_score
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link dependencies
target_link_libraries(MaratonaScoreLib
    PRIVATE
        OpenXLSX::OpenXLSX
        yaml-cpp
)

# Export symbols for Windows DLL
if(WIN32)
    target_compile_definitions(MaratonaScoreLib
        PRIVATE MARATONASCORE_EXPORTS
        INTERFACE MARATONASCORE_IMPORTS
    )
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(MaratonaScoreLib PRIVATE /W4)
else()
    target_compile_options(MaratonaScoreLib PRIVATE -Wall -Wextra -pedantic)
endif()
