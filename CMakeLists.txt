cmake_minimum_required(VERSION 3.0)
project(px-software)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)
set(QT5_LIBRARIES Qt5::Widgets Qt5::Sql)

set(SOURCES
        src/main.cpp
        src/GUI/MainWindow.cpp
        )

set(UI_FILES
        src/GUI/MainWindow.ui
        )
qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_FILES})

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${UI_GENERATED_HEADERS})
target_link_libraries(${CMAKE_PROJECT_NAME} ${QT5_LIBRARIES})
