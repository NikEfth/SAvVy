add_executable(test_display_container "${CMAKE_CURRENT_LIST_DIR}/test_display_container.cpp")

add_test(NAME test_display_container COMMAND test_display_container)

target_link_libraries(test_display_container PUBLIC Qt5::Core Qt5::Widgets Qt5::Test savvy_display IO buildblock)
