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

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Fetch nlohmann/json library
include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Add subdirectories for libraries
add_subdirectory(src/core)
add_subdirectory(src/cli)

# Main executable
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
