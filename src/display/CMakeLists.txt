target_sources(savvy_display
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/common_display.h"
    "${CMAKE_CURRENT_LIST_DIR}/display_container.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/display_container.h"
    "${CMAKE_CURRENT_LIST_DIR}/display_container_1d.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/display_container_1d.h"
    "${CMAKE_CURRENT_LIST_DIR}/display_container_2d.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/display_container_2d.h"
    )

include_directories( ${Qwt_INCLUDE_DIRS})
link_directories(${Qwt_LIBRARIES_DIR})
    
#target_include_directories(savvy_gui PUBLIC "${SAVVY_INCLUDE_DIR}")
target_link_libraries(savvy_display PUBLIC Qt5::Core Qt5::Widgets Qt5::Gui qwt)
 
