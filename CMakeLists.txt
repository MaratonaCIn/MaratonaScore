cmake_minimum_required(VERSION 3.14)

project(MaratonaScore
    VERSION 2.0.0
    DESCRIPTION "MaratonaCIn Rating System - Point-Based Scoring"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options
option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_GUI "Build GUI application" ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Fetch dependencies
include(FetchContent)

# Fetch nlohmann/json library
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Find Qt6 if building GUI
if(BUILD_GUI)
    find_package(Qt6 COMPONENTS Core Widgets QUIET)
    if(NOT Qt6_FOUND)
        message(STATUS "Qt6 not found in system, will try to use FetchContent")
        # Note: FetchContent for Qt6 is complex and not recommended
        # User should install Qt6 manually via package manager
        set(BUILD_GUI OFF)
        message(WARNING "Qt6 not found. GUI will not be built. Install Qt6 to enable GUI.")
    endif()
endif()

# Add subdirectories for libraries
add_subdirectory(src/core)
add_subdirectory(src/cli)

# Add GUI if Qt6 is available
if(BUILD_GUI AND Qt6_FOUND)
    add_subdirectory(src/gui)
endif()

# Main CLI executable
add_executable(maratona_score src/main.cpp)

# Link libraries to main executable
target_link_libraries(maratona_score
    PRIVATE
        MaratonaScore_core
        MaratonaScore_cli
        nlohmann_json::nlohmann_json
)

# Installation rules
install(TARGETS maratona_score
    RUNTIME DESTINATION bin
)

# Optional: Add tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Print build information
message(STATUS "===========================================")
message(STATUS "MaratonaScore Configuration")
message(STATUS "===========================================")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build Tests: ${BUILD_TESTS}")
message(STATUS "===========================================")
