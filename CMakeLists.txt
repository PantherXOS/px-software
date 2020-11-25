cmake_minimum_required(VERSION 3.0)
project(px-software)

IF(APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
ENDIF(APPLE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(px-gui-lib
                    src/GUI
                    src/GUI/viewitems
                    src/GUI/px-qobjects
                    src/PKG)

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

#add_definitions(-DFORCE_ZLIB_USAGE)

if(FORCE_ZLIB_USAGE)
    message(STATUS "Build with zlib as workaround zlib conflicting.")
    add_definitions(-DFORCE_ZLIB_USAGE)
endif()

IF(DEV_DB)
    message(STATUS "Using ./SAMPLE_DB instead of released version of internal DB in package repository.")
    ADD_DEFINITIONS(-DDEV_DB="./SAMPLE_DB")
ENDIF()

set(TS_FILE_LIST    src/GUI/translations/px-software_fa.ts
                    src/GUI/translations/px-software_de.ts
                    src/GUI/translations/px-software_fr.ts)
FILE(GLOB TS_FILES  ${TS_FILE_LIST})
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILE_LIST})
add_custom_target(translations ALL DEPENDS ${QM_FILES})
qt5_add_translation(QT_TR_FILES ${TS_FILES})

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
        px-gui-lib/PXWidget.h
        px-gui-lib/PXStatusBar.h)
        
FILE(GLOB GUI_SOURCES
        resources.qrc
        ${QT_TR_FILES}
        src/GUI/MainWindow.cpp src/GUI/MainWindow.h
        src/GUI/CategoryWidget.cpp src/GUI/CategoryWidget.h
        src/GUI/PackageListWidgetItem.cpp src/GUI/PackageListWidgetItem.h
        src/GUI/FileDownloader.cpp src/GUI/FileDownloader.h
        src/GUI/PackageManagerTracker.cpp src/GUI/PackageManagerTracker.h
        src/GUI/TerminalWidget.cpp src/GUI/TerminalWidget.h
        src/GUI/viewitems/InstalledPackageListView.cpp src/GUI/viewitems/InstalledPackageListView.h
        src/GUI/viewitems/UserUpdatablePackageListView.cpp src/GUI/viewitems/UserUpdatablePackageListView.h
        src/GUI/viewitems/SystemUpdatablePackageListView.cpp src/GUI/viewitems/SystemUpdatablePackageListView.h
        src/GUI/viewitems/InProgressPackageListView.cpp src/GUI/viewitems/InProgressPackageListView.h
        src/GUI/PackageDetails.cpp src/GUI/PackageDetails.h
        src/GUI/px-qobjects/QProgressIndicator.cpp src/GUI/px-qobjects/QProgressIndicator.h
        src/GUI/PackageComponent.h
        src/GUI/ScreenshotItem.h
        src/GUI/TagPackageList.h
        src/GUI/SearchPackagesList.h
        src/GUI/PackageListWidget.h
        src/GUI/PxQListUpdateWidgetItem.h
        src/GUI/px-qobjects/PxQPushButton.h
        src/GUI/Settings.h
        src/GUI/ScreenShotViewer.h
        src/GUI/UserUpdateNotification.h
        src/GUI/CacheManager.cpp src/GUI/CacheManager.h
        src/GUI/notification/pxnotification.cpp src/GUI/notification/pxnotification.h src/GUI/notification/pxnotification_p.h
        src/GUI/notification/notifications_interface.cpp src/GUI/notification/notifications_interface.h
        src/GUI/viewitems/UpdatesItem.h)

set(SOURCES
        src/RecDB.cpp
        src/PKG/DataEntities.cpp
        src/PKG/DataAccessLayer.cpp
        src/AsyncTaskRunner.cpp
        src/PKG/PackageManager.cpp
        src/PKG/GuixWrapper.cpp
        src/PKG/GUIX/GuixTask.cpp
        src/PKG/GUIX/PxTask.cpp
        src/PKG/GUIX/GuixProfileUpdateThread.cpp
        src/PKG/GUIX/GuixProfileUpdateTask.cpp
        src/PKG/GUIX/GuixInstalledPackagesTask.cpp
        src/PKG/GUIX/GuixPackageInstallTask.cpp
        src/PKG/GUIX/GuixPackageUpgradeTask.cpp
        src/PKG/GUIX/GuixPackageRemoveTask.cpp
        src/PKG/GUIX/GuixUpgradablePackagesTask.cpp
        src/PKG/GUIX/GuixProfileStatusTask.cpp
        src/PKG/MISC/LxqtSessionInterface.cpp
        src/PKG/MISC/Utils.cpp)

set(LIB_TARGET _${CMAKE_PROJECT_NAME})
add_library(${LIB_TARGET} ${SOURCES})
target_link_libraries(${LIB_TARGET} ${QT5_LIBRARIES} rec)
install(TARGETS ${LIB_TARGET} DESTINATION lib)

set(PX_GUI_LIB_TARGET px-gui-lib)
add_library(${PX_GUI_LIB_TARGET} SHARED ${PX_GUI_LIB_SOURCES})
target_link_libraries(${PX_GUI_LIB_TARGET} ${QT5_LIBRARIES})
install(TARGETS ${PX_GUI_LIB_TARGET} DESTINATION lib)

set(EXE_TARGET ${CMAKE_PROJECT_NAME})
add_executable(${EXE_TARGET}  ${GUI_SOURCES} src/main.cpp)
target_link_libraries(${EXE_TARGET} z ${QT5_LIBRARIES} ${LIB_TARGET} ${PX_GUI_LIB_TARGET} stdc++fs)
install(TARGETS ${EXE_TARGET} DESTINATION bin)
install(FILES ${QM_FILES} DESTINATION share/px-software/translations)
install(FILES desktop/px-software.desktop DESTINATION share/applications)
install(FILES script/px-software-update-check.scm DESTINATION bin)
install(FILES script/px-software-update-check DESTINATION bin)


IF(DEV_TR)
    message(STATUS "Using local translation files.")
    ADD_DEFINITIONS(-DDEV_TR="./translations")
    file(COPY ${QM_FILES} DESTINATION translations)
ENDIF()

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

px_software_test(RecDB              tests/TestRecDB.cpp)
px_software_test(DataAccessLayer    tests/TestDataAccessLayer.cpp)
px_software_test(AsyncTaskRunner    tests/TestAsyncTaskRunner.cpp)
px_software_test(PackageManager     tests/TestPackageManager.cpp)
