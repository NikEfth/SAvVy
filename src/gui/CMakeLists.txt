target_sources(savvy_gui
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/savvywindow.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/savvywindow.ui"
    "${CMAKE_CURRENT_LIST_DIR}/savvywindow.h"
    "${CMAKE_CURRENT_LIST_DIR}/panel_opened_files.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/panel_opened_files.ui"
    "${CMAKE_CURRENT_LIST_DIR}/panel_opened_files.h"
    "${CMAKE_CURRENT_LIST_DIR}/panel_opened_file_controls.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/panel_opened_file_controls.ui"
    "${CMAKE_CURRENT_LIST_DIR}/panel_opened_file_controls.h"
    "${CMAKE_CURRENT_LIST_DIR}/controls2dimage.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/controls2dimage.ui"
    "${CMAKE_CURRENT_LIST_DIR}/controls2dimage.h"
    "${CMAKE_CURRENT_LIST_DIR}/controls3dimage.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/controls3dimage.ui"
    "${CMAKE_CURRENT_LIST_DIR}/controls3dimage.h"
    )
    
include_directories( ${Stir_INCLUDE_DIRS})
link_directories(${Stir_LIBRARIES_DIR})

#target_include_directories(savvy_gui PUBLIC "${SAVVY_INCLUDE_DIR}")
target_link_libraries(savvy_gui PUBLIC Qt5::Core Qt5::Widgets Qt5::Gui buildblock)
