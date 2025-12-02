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

cmake_minimum_required(VERSION 3.15)

project(MaratonaScore
    VERSION 3.0.0
    DESCRIPTION "MaratonaCIn Rating System - Modern CLI and Library"
    LANGUAGES CXX
)

# ============================================================================
# C++ Standard Configuration
# ============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Build Configuration
# ============================================================================
set(CMAKE_DEBUG_POSTFIX d)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ============================================================================
# Build Options
# ============================================================================
option(BUILD_CLI "Build CLI application" ON)
option(BUILD_GUI "Build GUI application" OFF)
option(BUILD_LEGACY "Build legacy CLI (deprecated)" ON)
option(BUILD_TESTS "Build unit tests" OFF)

# ============================================================================
# Dependency Management with FetchContent
# ============================================================================
include(FetchContent)

# ----------------------------------------------------------------------------
# OpenXLSX - Excel file reading library
# ----------------------------------------------------------------------------
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
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Disable yaml-cpp tests" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "Disable yaml-cpp tools" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "Disable yaml-cpp contrib" FORCE)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG        master
)

# ----------------------------------------------------------------------------
# CLI11 - Modern C++ command line parser
# ----------------------------------------------------------------------------
if (BUILD_CLI)
    FetchContent_Declare(
        CLI11
        GIT_REPOSITORY https://github.com/CLI11/CLI11.git
        GIT_TAG        v2.4.2
    )
endif()

# ----------------------------------------------------------------------------
# nlohmann/json - JSON for Modern C++
# ----------------------------------------------------------------------------
# TODO: Descomentar quando implementar CLI moderno
# FetchContent_Declare(
#     nlohmann_json
#     GIT_REPOSITORY https://github.com/nlohmann/json.git
#     GIT_TAG        v3.11.3
# )

# Make all dependencies available
FetchContent_MakeAvailable(OpenXLSX yaml-cpp CLI11)

# ============================================================================
# Project Subdirectories
# ============================================================================

# Core library (always built)
add_subdirectory(lib)

# CLI application
if(BUILD_CLI)
    add_subdirectory(cli)
endif()

# GUI application (future)
if(BUILD_GUI)
    add_subdirectory(gui)
endif()

# Legacy CLI (deprecated)
if(BUILD_LEGACY)
    add_subdirectory(legacy)
endif()

# Unit tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ============================================================================
# Installation Rules
# ============================================================================

# Install library
install(TARGETS MaratonaScoreLib
    RUNTIME DESTINATION bin     # DLL goes to bin/ on Windows
    LIBRARY DESTINATION lib     # .so goes to lib/ on Linux
    ARCHIVE DESTINATION lib     # Import .lib stays in lib/
)

# Install library headers
install(DIRECTORY lib/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)

# Install CLI (uncomment when implemented)
# if(BUILD_CLI)
#     install(TARGETS MaratonaScoreCLI
#         RUNTIME DESTINATION bin
#     )
# endif()

# Install GUI (uncomment when implemented)
# if(BUILD_GUI)
#     install(TARGETS MaratonaScoreGUI
#         RUNTIME DESTINATION bin
#     )
# endif()

# Install legacy CLI
if(BUILD_LEGACY)
    install(TARGETS maratona_score
        RUNTIME DESTINATION bin
    )
endif()

# Install templates
install(DIRECTORY templates/
    DESTINATION share/maratona_score/templates
)

# ============================================================================
# Build Summary
# ============================================================================
message(STATUS "===========================================")
message(STATUS "MaratonaScore Build Configuration")
message(STATUS "===========================================")
message(STATUS "Version:      ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type:   ${CMAKE_BUILD_TYPE}")
message(STATUS "")
message(STATUS "Components:")
message(STATUS "  Library:    Always")
message(STATUS "  CLI:        ${BUILD_CLI}")
message(STATUS "  GUI:        ${BUILD_GUI}")
message(STATUS "  Legacy CLI: ${BUILD_LEGACY}")
message(STATUS "  Tests:      ${BUILD_TESTS}")
message(STATUS "===========================================")
message(STATUS "Output Directories:")
message(STATUS "  Executables: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "  Libraries:   ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "===========================================")
