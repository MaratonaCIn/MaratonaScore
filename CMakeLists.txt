cmake_minimum_required(VERSION 3.15)

project(MaratonaScore
    VERSION 2.0.0
    DESCRIPTION "MaratonaCIn Rating System - Point-Based Scoring"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add 'd' suffix for debug builds
set(CMAKE_DEBUG_POSTFIX d)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options
option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_GUI "Build GUI application" ON)

# Fetch dependencies
include(FetchContent)

# Disable OpenXLSX demos, tests, and documentation
set(OPENXLSX_CREATE_DOCS OFF CACHE BOOL "" FORCE)
set(OPENXLSX_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(OPENXLSX_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(OPENXLSX_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    OpenXLSX
    GIT_REPOSITORY https://github.com/troldal/OpenXLSX.git
    GIT_TAG        master
)

# Disable yaml-cpp tests and tools
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG        master
)

FetchContent_MakeAvailable(OpenXLSX yaml-cpp)

add_subdirectory(src)

# Installation rules
install(TARGETS maratona_score
    RUNTIME DESTINATION bin
)

# Install the library
install(TARGETS MaratonaScoreLib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install headers (for library usage)
install(DIRECTORY src/include/
    DESTINATION include/maratona_score
    FILES_MATCHING PATTERN "*.hpp"
)

# Install template files
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

# Optional: Add tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Print build information (sem alteração)
message(STATUS "===========================================")
message(STATUS "MaratonaScore Configuration")
message(STATUS "===========================================")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

message(STATUS "Build Tests: ${BUILD_TESTS}")
message(STATUS "===========================================")