target_sources(savvy_IO
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/io_manager.h")

include_directories( ${Stir_INCLUDE_DIRS})
link_directories(${Stir_LIBRARIES_DIR})

find_package(ITK QUIET)

if (ITK_FOUND)
    message(STATUS "ITK libraries added.")
    set(HAVE_ITK ON)
    include(${ITK_USE_FILE})
    find_library( ITK_LIBRARIES NAMES ITKCommon IMPORTED)
else()
    message(STATUS "ITK support disabled.")
    set(HAVE_ITK OFF)
endif()

target_link_libraries(savvy_IO PUBLIC Qt5::Core IO buildblock ITKCommon ${ITK_LIBRARIES})
