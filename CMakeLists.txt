cmake_minimum_required(VERSION 3.0)
project(px-software)

IF(APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
ENDIF(APPLE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(src/GUI
                    src/PKG)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Test REQUIRED)
set(QT5_LIBRARIES Qt5::Widgets Qt5::Network Qt5::Sql)
set(QT5_TEST_LIBRARIES Qt5::Core Qt5::Test)


FILE(GLOB GUI_SOURCES
        resources.qrc
        src/GUI/MainWindow.cpp src/GUI/MainWindow.h
        src/GUI/CategoryWidget.cpp src/GUI/CategoryWidget.h
        src/GUI/PxQListWidgetItem.h
        src/GUI/ContentList.cpp src/GUI/ContentList.h
        src/GUI/PxQWidget.h
        )

set(SOURCES
        src/RecDB.cpp
        src/PKG/DataEntities.cpp
        src/PKG/DataAccessLayer.cpp
        )

set(LIB_TARGET _${CMAKE_PROJECT_NAME})
add_library(${LIB_TARGET} ${SOURCES})
target_link_libraries(${LIB_TARGET} ${QT5_LIBRARIES} rec)

set(EXE_TARGET ${CMAKE_PROJECT_NAME})
add_executable(${EXE_TARGET}  ${GUI_SOURCES} src/main.cpp)
target_link_libraries(${EXE_TARGET} ${LIB_TARGET} ${QT5_LIBRARIES})

enable_testing(true)
file(COPY SAMPLE_DB DESTINATION ${CMAKE_BINARY_DIR}/)

# px_software_test(title [sources...])
macro(px_software_test title)
    set(test_target test_${title})
    add_executable(${test_target} ${ARGN})
    target_include_directories(${test_target} PUBLIC src)
    target_link_libraries(${test_target} ${QT5_TEST_LIBRARIES} ${LIB_TARGET})
    add_test(${test_target} ${test_target})
endmacro()

px_software_test(RecDB tests/TestRecDB.cpp)
px_software_test(DataAccessLayer tests/TestDataAccessLayer.cpp)
