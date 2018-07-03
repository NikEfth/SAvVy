target_sources(savvy_gui
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/savvywindow.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/savvywindow.ui"
    "${CMAKE_CURRENT_LIST_DIR}/savvywindow.h"
    "${CMAKE_CURRENT_LIST_DIR}/savvy_settings.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/savvy_settings.ui"
    "${CMAKE_CURRENT_LIST_DIR}/savvy_settings.h"
    "${CMAKE_CURRENT_LIST_DIR}/panel_opened_files.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/panel_opened_files.ui"
    "${CMAKE_CURRENT_LIST_DIR}/panel_opened_files.h"
    "${CMAKE_CURRENT_LIST_DIR}/panel_opened_file_controls.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/panel_opened_file_controls.ui"
    "${CMAKE_CURRENT_LIST_DIR}/panel_opened_file_controls.h"
    "${CMAKE_CURRENT_LIST_DIR}/controls_display_1d.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/controls_display_1d.ui"
    "${CMAKE_CURRENT_LIST_DIR}/controls_display_1d.h"
    "${CMAKE_CURRENT_LIST_DIR}/controls_display_2d.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/controls_display_2d.ui"
    "${CMAKE_CURRENT_LIST_DIR}/controls_display_2d.h"
    )
    
include_directories( ${Stir_INCLUDE_DIRS})
link_directories(${Stir_LIBRARIES_DIR})

#target_include_directories(savvy_gui PUBLIC "${SAVVY_INCLUDE_DIR}")
target_link_libraries(savvy_gui PUBLIC Qt5::Core Qt5::Widgets Qt5::Gui buildblock)
