file(GLOB_RECURSE MARATONA_SCORE_SOURCES
    CONFIGURE_DEPENDS
    "src/models/*.cpp"
    "src/parser/*.cpp"
    "src/utils/*.cpp"
    "src/score/*.cpp"
)

add_library(MaratonaScoreLib SHARED
    ${MARATONA_SCORE_SOURCES}
)

# Set debug postfix and output directories for the library
set_target_properties(MaratonaScoreLib PROPERTIES
    DEBUG_POSTFIX d
    PREFIX ""  # Remove o prefixo "lib" no Windows/Linux
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin  # DLL vai para bin/ no Windows
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin  # .so vai para bin/ no Linux
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib  # .lib de importação fica em lib/
)

# Define export symbols for DLL
target_compile_definitions(MaratonaScoreLib
    PRIVATE
        MARATONASCORE_EXPORTS  # Define when building the DLL
    INTERFACE
        $<$<PLATFORM_ID:Windows>:MARATONASCORE_DLL>  # Define for users on Windows
)

# Set symbol visibility to hidden (forces explicit exports)
# This makes GCC/Clang behave like MSVC (default hidden)
set_target_properties(MaratonaScoreLib PROPERTIES
    CXX_VISIBILITY_PRESET hidden       # Hide symbols by default
    VISIBILITY_INLINES_HIDDEN ON       # Hide inline functions too
)

# --- 3. Definir os Include Path da Biblioteca ---
target_include_directories(MaratonaScoreLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MaratonaScoreLib
    PRIVATE
        OpenXLSX::OpenXLSX
        yaml-cpp::yaml-cpp
)

add_executable(maratona_score main.cpp)

# Set debug postfix and output directory for the executable
set_target_properties(maratona_score PROPERTIES
    DEBUG_POSTFIX d
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin  # Executável vai para bin/
)

target_link_libraries(maratona_score
    PRIVATE
        MaratonaScoreLib
)

# Copy dependency DLLs to bin/ directory on Windows (post-build)
if(WIN32)
    add_custom_command(TARGET maratona_score POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:maratona_score>
            $<TARGET_FILE_DIR:maratona_score>
        COMMAND_EXPAND_LISTS
        COMMENT "Copying DLL dependencies to bin/"
    )
endif()