cmake_minimum_required(VERSION 3.0)
project(px-gui-lib)

message(STATUS "Building px-gui-lib ...")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(px-gui-lib)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

set(QT5_LIBRARIES Qt5::Widgets Qt5::Network Qt5::Sql Qt5::DBus)
set(QT5_TEST_LIBRARIES Qt5::Core Qt5::Test)

FILE(GLOB PX_GUI_LIB_SOURCES
        px-gui-lib/PXWindowManager.cpp px-gui-lib/PXWindowManager.h
        px-gui-lib/PXMainWindow.cpp px-gui-lib/PXMainWindow.h
        px-gui-lib/PXSearchBox.h
        px-gui-lib/PXSideBar.h
        px-gui-lib/PXSeperator.h
        px-gui-lib/PXSideBarItem.cpp px-gui-lib/PXSideBarItem.h
        px-gui-lib/PXTopBar.cpp px-gui-lib/PXTopBar.h
        px-gui-lib/PXParamSettings.h
        px-gui-lib/PXContentWidget.h
        px-gui-lib/PXStatusBar.h)

set(PX_GUI_LIB_TARGET px-gui-lib)
add_library(${PX_GUI_LIB_TARGET} SHARED ${PX_GUI_LIB_SOURCES})
target_link_libraries(${PX_GUI_LIB_TARGET} ${QT5_LIBRARIES})
install(TARGETS ${PX_GUI_LIB_TARGET} DESTINATION lib)