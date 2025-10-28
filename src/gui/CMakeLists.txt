# GUI Library
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# GUI sources
set(GUI_SOURCES
    MainWindow.cpp
    RankingTableWidget.cpp
    ContestsListWidget.cpp
    ContestDetailWidget.cpp
    AddContestDialog.cpp
    BlacklistDialog.cpp
    ConfigDialog.cpp
)

# GUI headers
set(GUI_HEADERS
    ${CMAKE_SOURCE_DIR}/include/MaratonaScore/gui/MainWindow.h
    ${CMAKE_SOURCE_DIR}/include/MaratonaScore/gui/RankingTableWidget.h
    ${CMAKE_SOURCE_DIR}/include/MaratonaScore/gui/ContestsListWidget.h
    ${CMAKE_SOURCE_DIR}/include/MaratonaScore/gui/ContestDetailWidget.h
    ${CMAKE_SOURCE_DIR}/include/MaratonaScore/gui/AddContestDialog.h
    ${CMAKE_SOURCE_DIR}/include/MaratonaScore/gui/BlacklistDialog.h
    ${CMAKE_SOURCE_DIR}/include/MaratonaScore/gui/ConfigDialog.h
)

# Create GUI library
add_library(MaratonaScore_gui ${GUI_SOURCES} ${GUI_HEADERS})

target_link_libraries(MaratonaScore_gui
    PUBLIC
        Qt6::Core
        Qt6::Widgets
        MaratonaScore_core
        nlohmann_json::nlohmann_json
)

target_include_directories(MaratonaScore_gui
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# GUI executable
add_executable(maratona_score_gui main_gui.cpp)

target_link_libraries(maratona_score_gui
    PRIVATE
        MaratonaScore_gui
        MaratonaScore_core
        Qt6::Core
        Qt6::Widgets
)

# Set Windows subsystem to GUI (not console)
if(WIN32)
    set_target_properties(maratona_score_gui PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()

# Installation
install(TARGETS maratona_score_gui
    RUNTIME DESTINATION bin
)
