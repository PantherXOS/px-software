cmake_minimum_required(VERSION 3.0)
project(px-software)

IF(APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
ENDIF(APPLE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Test REQUIRED)
set(QT5_LIBRARIES Qt5::Widgets Qt5::Sql)
set(QT5_TEST_LIBRARIES Qt5::Core Qt5::Test)

set(SOURCES
        src/GUI/MainWindow.cpp
        src/RecDB.cpp
        src/PKG/DataEntities.cpp
        src/PKG/DataAccessLayer.cpp
        )

set(UI_FILES
        src/GUI/MainWindow.ui
        )
qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_FILES})

set(LIB_TARGET _${CMAKE_PROJECT_NAME})
add_library(${LIB_TARGET} ${SOURCES} ${UI_GENERATED_HEADERS})
target_link_libraries(${LIB_TARGET} ${QT5_LIBRARIES} rec)

set(EXE_TARGET ${CMAKE_PROJECT_NAME})
add_executable(${EXE_TARGET} src/main.cpp)
target_link_libraries(${EXE_TARGET} ${LIB_TARGET})


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
