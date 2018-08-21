
# Find the Qt components
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# -- This is required in order to build QT elements
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Check for libBoard for high quality printing

target_sources(savvy_tools
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/ToolManager.h"
    "${CMAKE_CURRENT_LIST_DIR}/ToolManager.inl"
    "${CMAKE_CURRENT_LIST_DIR}/ToolManager.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/ToolManager.ui"
    "${CMAKE_CURRENT_LIST_DIR}/tool.h"
    "${CMAKE_CURRENT_LIST_DIR}/tool.cpp"
    )

target_link_libraries(savvy_tools PUBLIC Qt5::Core Qt5::Widgets Qt5::Gui)

# target_include_directories(savvy_tools PUBLIC "${ALGLIB_BASE}" "${CMAKE_CURRENT_LIST_DIR}")
