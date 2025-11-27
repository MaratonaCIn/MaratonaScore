#    Copyright 2025 MaratonaCIn
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.

# ============================================================================
# MaratonaScore Library and Executable Build
# ============================================================================
# This file builds:
#   1. MaratonaScoreLib - Shared library (DLL/.so) with all business logic
#   2. maratona_score - Thin executable that links to the library
#
# Architecture benefits:
#   - Library can be reused in other projects (GUI, tests, plugins)
#   - Clean separation between logic and entry point
#   - Easier testing (mock main, test library directly)
# ============================================================================

# ============================================================================
# 1. Shared Library: MaratonaScoreLib
# ============================================================================

# Collect all implementation files
# CONFIGURE_DEPENDS ensures CMake re-runs when files are added/removed
file(GLOB_RECURSE MARATONA_SCORE_SOURCES
    CONFIGURE_DEPENDS
    "src/models/*.cpp"
    "src/parser/*.cpp"
    "src/utils/*.cpp"
    "src/score/*.cpp"
)

# Create shared library target
add_library(MaratonaScoreLib SHARED
    ${MARATONA_SCORE_SOURCES}
)

# ----------------------------------------------------------------------------
# Library Properties
# ----------------------------------------------------------------------------
set_target_properties(MaratonaScoreLib PROPERTIES
    DEBUG_POSTFIX d                     # Append 'd' for debug builds
    PREFIX ""                           # Remove "lib" prefix (libMaratonaScoreLib → MaratonaScoreLib)

    # Output directories
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin  # DLL location (Windows)
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin  # .so location (Linux/macOS)
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib  # Import .lib location (Windows)

    # Symbol visibility (hide symbols by default, export only MARATONASCORE_API)
    CXX_VISIBILITY_PRESET hidden           # Hide all symbols by default
    VISIBILITY_INLINES_HIDDEN ON           # Hide inline functions too
)

# ----------------------------------------------------------------------------
# DLL Export/Import Definitions
# ----------------------------------------------------------------------------
# Define symbols that control __declspec(dllexport/dllimport) in export.hpp
target_compile_definitions(MaratonaScoreLib
    PRIVATE
        # When building the library, export symbols
        MARATONASCORE_EXPORTS
    INTERFACE
        # When using the library on Windows, import symbols
        $<$<PLATFORM_ID:Windows>:MARATONASCORE_DLL>
)

# ----------------------------------------------------------------------------
# Include Directories
# ----------------------------------------------------------------------------
target_include_directories(MaratonaScoreLib
    PUBLIC
        # Public headers (available to library users)
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        # Private headers (internal use only)
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# ----------------------------------------------------------------------------
# Link Dependencies
# ----------------------------------------------------------------------------
# Link OpenXLSX and yaml-cpp PRIVATELY (statically linked into the library)
# This hides these dependencies from library users
target_link_libraries(MaratonaScoreLib
    PRIVATE
        OpenXLSX::OpenXLSX
        yaml-cpp::yaml-cpp
)

# ============================================================================
# 2. Executable: maratona_score
# ============================================================================

# Create executable from main.cpp only
add_executable(maratona_score main.cpp)

# ----------------------------------------------------------------------------
# Executable Properties
# ----------------------------------------------------------------------------
set_target_properties(maratona_score PROPERTIES
    DEBUG_POSTFIX d                            # Append 'd' for debug builds
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin  # Output to bin/
)

# ----------------------------------------------------------------------------
# Link to Library
# ----------------------------------------------------------------------------
# Link against our shared library (MaratonaScoreLib.dll/.so)
target_link_libraries(maratona_score
    PRIVATE
        MaratonaScoreLib
)

# ============================================================================
# 3. Post-Build: Copy DLL Dependencies (Windows Only)
# ============================================================================
# On Windows, copy runtime DLLs next to the executable for easy execution
# This includes MaratonaScoreLib.dll and any other runtime dependencies
if(WIN32)
    add_custom_command(TARGET maratona_score POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:maratona_score>
            $<TARGET_FILE_DIR:maratona_score>
        COMMAND_EXPAND_LISTS
        COMMENT "Copying runtime DLL dependencies to bin/"
    )
endif()

# ============================================================================
# Notes for Developers
# ============================================================================
#
# Adding New Source Files:
#   - Headers go in include/ (public) or src/ (private)
#   - Implementations go in src/<category>/
#   - GLOB_RECURSE will automatically pick up new .cpp files
#   - Remember to use MARATONASCORE_API for public classes/functions
#
# Symbol Visibility:
#   - All symbols are hidden by default (CXX_VISIBILITY_PRESET hidden)
#   - Only classes/functions marked with MARATONASCORE_API are exported
#   - This prevents symbol conflicts and reduces binary size
#
# Linking Strategy:
#   - OpenXLSX and yaml-cpp are PRIVATE dependencies (statically linked)
#   - Users of MaratonaScoreLib don't need to link these directly
#   - Only MaratonaScoreLib needs to be distributed with the executable
#
# ============================================================================
