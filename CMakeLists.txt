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
# MaratonaScore - CMake Build Configuration
# ============================================================================
# This is a library + executable project:
#   - MaratonaScoreLib: Shared library containing all business logic
#   - maratona_score: Thin executable wrapper
#
# The library statically links OpenXLSX and yaml-cpp to avoid runtime
# dependency conflicts.
# ============================================================================

cmake_minimum_required(VERSION 3.15)

project(MaratonaScore
    VERSION 2.0.0
    DESCRIPTION "MaratonaCIn Rating System - Point-Based Scoring"
    LANGUAGES CXX
)

# ============================================================================
# C++ Standard Configuration
# ============================================================================
# Require C++20 for modern features (concepts, ranges, etc.)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Disable compiler-specific extensions

# ============================================================================
# Build Configuration
# ============================================================================
# Add 'd' suffix to debug binaries (e.g., maratona_scored.exe)
set(CMAKE_DEBUG_POSTFIX d)

# Output directories for generated binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  # Executables and DLLs
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  # Shared libraries (.so)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  # Static libraries (.a, .lib)

# ============================================================================
# Build Options
# ============================================================================
option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_GUI "Build GUI application" ON)

# ============================================================================
# Dependency Management with FetchContent
# ============================================================================
# Dependencies are automatically downloaded and built during configuration
include(FetchContent)

# ----------------------------------------------------------------------------
# OpenXLSX - Excel file reading library
# ----------------------------------------------------------------------------
# Disable unnecessary components to speed up build
set(OPENXLSX_CREATE_DOCS OFF CACHE BOOL "Disable OpenXLSX documentation" FORCE)
set(OPENXLSX_BUILD_SAMPLES OFF CACHE BOOL "Disable OpenXLSX samples" FORCE)
set(OPENXLSX_BUILD_TESTS OFF CACHE BOOL "Disable OpenXLSX tests" FORCE)
set(OPENXLSX_BUILD_BENCHMARKS OFF CACHE BOOL "Disable OpenXLSX benchmarks" FORCE)

FetchContent_Declare(
    OpenXLSX
    GIT_REPOSITORY https://github.com/troldal/OpenXLSX.git
    GIT_TAG        master
)

# ----------------------------------------------------------------------------
# yaml-cpp - YAML configuration file parser
# ----------------------------------------------------------------------------
# Disable unnecessary components
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Disable yaml-cpp tests" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "Disable yaml-cpp tools" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "Disable yaml-cpp contrib" FORCE)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG        master
)

# Make dependencies available for linking
FetchContent_MakeAvailable(OpenXLSX yaml-cpp)

# ============================================================================
# Project Subdirectories
# ============================================================================
# Contains library and executable targets
add_subdirectory(src)

# ============================================================================
# Installation Rules
# ============================================================================
# These rules define how 'cmake --install' behaves

# Install executable to bin/
install(TARGETS maratona_score
    RUNTIME DESTINATION bin
)

# Install shared library to bin/ (Windows DLL) or lib/ (Linux .so)
install(TARGETS MaratonaScoreLib
    RUNTIME DESTINATION bin     # DLL goes to bin/ on Windows
    LIBRARY DESTINATION lib     # .so goes to lib/ on Linux
    ARCHIVE DESTINATION lib     # Import .lib stays in lib/
)

# Install public headers for library usage
# Allows other projects to link against MaratonaScoreLib
install(DIRECTORY src/include/
    DESTINATION include/maratona_score
    FILES_MATCHING PATTERN "*.hpp"
)

# Install template configuration files
install(FILES
    templates/settings/config.yaml
    templates/settings/blacklist.txt
    DESTINATION share/maratona_score/templates/settings
)

install(FILES
    templates/data/finals.txt
    DESTINATION share/maratona_score/templates/data
)

install(FILES
    templates/README.md
    DESTINATION share/maratona_score/templates
)

# ============================================================================
# Optional: Unit Tests
# ============================================================================
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ============================================================================
# Build Summary
# ============================================================================
# Print configuration information for user reference
message(STATUS "===========================================")
message(STATUS "MaratonaScore Build Configuration")
message(STATUS "===========================================")
message(STATUS "Version:      ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type:   ${CMAKE_BUILD_TYPE}")
message(STATUS "Build Tests:  ${BUILD_TESTS}")
message(STATUS "===========================================")
message(STATUS "Output Directories:")
message(STATUS "  Executables: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "  Libraries:   ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "===========================================")
