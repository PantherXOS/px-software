/*
 * px-software - Qt-GUI package manager for guix on PantherX OS
 * Copyright (C) 2019, 2020  Hamzeh Nasajpour <h.nasajpour@pantherx.org>
 * Copyright (C) 2019, 2020  Reza Alizadeh Majd <r.majd@pantherx.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#ifndef PX_SOFTWARE_SETTINGS_H
#define PX_SOFTWARE_SETTINGS_H

#include <QScreen>
#include <QDir>
#include <PXSettings.h>

#define CACHE_DIR                       QDir::homePath() + QString("/.cache/px/px-software/")
#define CACHE_EXPIRE_DAY                30

#define VIEW_MESSAGE_FONT_SIZE          16
#define VIEW_LOADING_ICON_SIZE          128

#define UPDATE_ITEM_LOADING_SIZE        24
#define UPDATE_ITEM_RICON_SIZE          20

#define CATEGORY_TITLE_FONT_SIZE        12
#define CATEGORY_ICON_CACHE_DIR         "categories/"
#define CATEGORY_ICON_SIZE              64
#define CATEGORY_ITEM_HEIGHT            CATEGORY_ICON_SIZE + 12
#define CATEGORY_ITEM_WIDTH             300

#define PACKAGE_DETAILS_LEFT_PANEL_SIZE 172

#define IN_PROGRESS_GIF_HEIGHT          4
#define PACKAGE_ICON_CACHE_DIR          "images/"
#define PACKAGE_BUTTON_W                PACKAGE_DETAILS_LEFT_PANEL_SIZE
#define PACKAGE_BUTTON_INPROGRESS_W     PACKAGE_BUTTON_W/2-5
#define PACKAGE_BUTTON_H                32
#define PACKAGE_ICON_SIZE               64
#define PACKAGE_SCREENSHOTS_CACHE_DIR   "images/"
#define PACKAGE_SCREENSHOT_W            480
#define PACKAGE_SCREENSHOT_H            360
#define PACKAGE_SCREENSHOT_SPACING      10
#define PACKAGE_SCREENSHOT_LOADING_SIZE 24

#define OTHER_PACKAGES_FONT_SIZE        28
#define OTHER_PACKAGES_DESC_FONT_SIZE   12
#define PACKAGE_TITLE_FONT_SIZE         12
#define PACKAGE_DESC_FONT_SIZE          10
#define OTHER_PACKAGE_DESC_STYLESHEET       "QLabel {background-color: transparent; color : gray; }"
#define PACKAGE_CANCEL_STYLESHEET           "QPushButton {background-color: " + PXSettings::Colors::redColor().name() + "; border: 0px; color: white;}"
#define PACKAGE_INSTALL_STYLESHEET          "QPushButton {background-color: " + PXSettings::Colors::greenColor().name() + "; border: 0px; color: white;}"
#define PACKAGE_UPDATE_STYLESHEET           "QPushButton {background-color: " + PXSettings::Colors::blueColor().name() + "; border: 0px; color: white;}"
#define PACKAGE_REMOVE_STYLESHEET           "QPushButton {background-color: " + PXSettings::Colors::redColor().name() + "; border: 0px; color: white;}"
#define PACKAGE_BUTTON_DISABLE_STYLESHEET   "QPushButton {background-color: #7C7C7C; border: 0px; color: white;}"
#define PACKAGE_INPROGRESS_STYLESHEET       "QPushButton {background-color: " + PXSettings::Colors::grayColor().name() + "; border: 0px; color: black;}\
                                             QPushButton:hover:!pressed {background-color: black; color: white}"
#define PACKAGE_ICON_STYLESHEET             "QLabel {background-color: transparent; border 1px solid rgb(80, 80, 80);}"
#define PACKAGE_LIST_LABELS_STYLESHEET      "QLabel {background-color: transparent;}"
#define PACKAGE_LICENSE_STYLESHEET          "QLabel {background-color: transparent; color : gray; }"
#define PACKAGE_BUTTON_INSTALLING           "Installing..."
#define PACKAGE_BUTTON_REMOVING             "Removing..."
#define PACKAGE_BUTTON_UPDATING             "Updating..."
#define PACKAGE_BUTTON_OBJ_NAME_INSTALL     "package_button_install"
#define PACKAGE_BUTTON_OBJ_NAME_REMOVE      "package_button_remove"
#define PACKAGE_BUTTON_OBJ_NAME_UPDATE      "package_button_update"
#define PACKAGE_DETAILS_RIGHT_PANEL_MARGIN  7
#define PACKAGE_DETAILS_RIGHT_PANEL_SPACE   15


#define ITEM_HOVER_STYLESHEET           "QWidget:hover{background: %1} QWidget{background: transparent}"

#define SCREENSHOT_ARROW_KEY_ICON_SIZE  128
#define SCREENSHOT_ARROW_KEY_STYLESHEET "QPushButton {border: 0px transparent;}"
#define SCREENSHOT_LIST_STYLESHEET      "QListWidget {border: 0px;}"
#define SCREENSHOT_NOT_AVAILABLE_MSG    "No images founds."
#define SCREENSHOT_MSG_FONT_SIZE        PACKAGE_DESC_FONT_SIZE
#define SCREENSHOT_PICTURE_SIZE_W       (QGuiApplication::primaryScreen()->availableGeometry().width() * 0.8)
#define SCREENSHOT_PICTURE_SIZE_H       SCREENSHOT_PICTURE_SIZE_W * 3 / 4

#define UPDATE_DB_MESSAGE_BEFORE_UPDATE  "Checking for software updates ..."
#define UPDATE_DB_MESSAGE_UPDATE_ERROR   "Check your internet connection for initializing the Software Database and re-open the Software"
#define UPDATE_DB_MESSAGE_FONT_SIZE      20

#define UPDATE_ALL_USER_PACKAGE_DESC    "Update all applications that you have installed yourself."
#define UPDATE_ALL_SYS_PACKAGE_DESC     "Update the operating system and all globally installed applications."

#define UPDATE_ALL_ITEM_SIZE_H          96
#define UPDATE_ITEM_BACKGROUND_COLOR    "#F3F1F1"
#endif //PX_SOFTWARE_SETTINGS_H
