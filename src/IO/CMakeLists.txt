target_sources(savvy_IO
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/io_manager.h")

include_directories( ${Stir_INCLUDE_DIRS})
link_directories(${Stir_LIBRARIES_DIR})

target_link_libraries(savvy_IO PUBLIC Qt5::Core IO buildblock)
