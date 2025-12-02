# ============================================================================
# MaratonaScore GUI
# ============================================================================
# Graphical user interface (future implementation)
# ============================================================================

# TODO: Implementar quando GUI for desenvolvido
# Placeholder para manter estrutura consistente

if(NOT BUILD_GUI)
    return()
endif()

message(STATUS "GUI build is enabled but not yet implemented")
message(STATUS "Skipping GUI target...")

# Future implementation:
# file(GLOB_RECURSE GUI_SOURCES
#     CONFIGURE_DEPENDS
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
# )
#
# add_executable(MaratonaScoreGUI ${GUI_SOURCES})
#
# target_include_directories(MaratonaScoreGUI
#     PRIVATE
#         ${CMAKE_CURRENT_SOURCE_DIR}/include
# )
#
# target_link_libraries(MaratonaScoreGUI
#     PRIVATE
#         MaratonaScoreLib
#         # Qt6::Widgets or wxWidgets or other GUI framework
# )
