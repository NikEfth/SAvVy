target_sources(savvy_display
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/common_display.h"
    "${CMAKE_CURRENT_LIST_DIR}/display_container.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/display_container.h"
    "${CMAKE_CURRENT_LIST_DIR}/display_manager.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/display_manager.h"
    "${CMAKE_CURRENT_LIST_DIR}/display_manager.ui"
    "${CMAKE_CURRENT_LIST_DIR}/ctrl_levels.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/ctrl_levels.h"
    "${CMAKE_CURRENT_LIST_DIR}/ctrl_levels.ui"
    "${CMAKE_CURRENT_LIST_DIR}/histogram_manager.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/histogram_manager.h"
    "${CMAKE_CURRENT_LIST_DIR}/histogram_manager.ui"
    "${CMAKE_CURRENT_LIST_DIR}/display_container_1d.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/display_container_1d.h"
    "${CMAKE_CURRENT_LIST_DIR}/display_container_raster.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/display_container_raster.h"
    "${CMAKE_CURRENT_LIST_DIR}/display_container_2d.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/display_container_2d.h"
    "${CMAKE_CURRENT_LIST_DIR}/display_container_3d.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/display_container_3d.h"
    "${CMAKE_CURRENT_LIST_DIR}/display_container_bar.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/display_container_bar.h")

include_directories(${Qwt_INCLUDE_DIRS})
link_directories(${Qwt_LIBRARIES_DIR})

if(UNIX OR APPLE)

    find_library(GSL_LIBRARY NAMES gsl IMPORTED)

    find_library(GSL_BLAS_LIBRARY NAMES gslcblas IMPORTED)

endif()

if(WIN32)

    find_library(GSL_LIBRARY NAMES gsl_ddl gsl_ddl_d IMPORTED)

    set(GSL_INCLUDE_DIR "default value" CACHE FILEPATH "description")

    include_directories(${GSL_INCLUDE_DIR})

    add_definitions(-D QWT_DLL)

endif()

target_link_libraries(savvy_display PUBLIC Qt5::Core Qt5::Widgets Qt5::Gui savvy_gui qwt ${GSL_LIBRARY} ${GSL_BLAS_LIBRARY})
