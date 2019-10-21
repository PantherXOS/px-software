cmake_minimum_required(VERSION 3.0)
project(px-software)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

######## QT-qmake settings
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Sql Core Gui Widgets)
qt5_add_resources(RESOURCES resources.qrc)
set(QT5_LIBRARIES Qt5::Widgets Qt5::Sql Qt5::Core Qt5::Gui)
########

set(SOURCES
        src/main.cpp
        src/GUI/MainWindow.cpp
        )

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${UI_GENERATED_HEADERS})
target_link_libraries(${CMAKE_PROJECT_NAME} ${QT5_LIBRARIES})
