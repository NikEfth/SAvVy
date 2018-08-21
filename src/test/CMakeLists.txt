add_executable(test_display_container "${CMAKE_CURRENT_LIST_DIR}/test_display_container.cpp")
add_executable(test_conversions "${CMAKE_CURRENT_LIST_DIR}/test_conversions.cpp")

add_test(NAME test_display_container COMMAND test_display_container)
add_test(NAME test_conversions COMMAND test_conversions)

target_link_libraries(test_display_container PUBLIC Qt5::Core Qt5::Widgets Qt5::Test savvy_display IO buildblock)
target_link_libraries(test_conversions PUBLIC Qt5::Core Qt5::Widgets Qt5::Test savvy_display IO buildblock)

target_sources(savvy_gui_tests
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/guitests.h"
    "${CMAKE_CURRENT_LIST_DIR}/guitests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/simple_guitests.h"
    "${CMAKE_CURRENT_LIST_DIR}/simple_guitests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/notsosimple_guitests.h"
    "${CMAKE_CURRENT_LIST_DIR}/notsosimple_guitests.cpp"
    )

target_link_libraries(savvy_gui_tests PUBLIC Qt5::Core Qt5::Widgets Qt5::Gui Qt5::PrintSupport qwt)
