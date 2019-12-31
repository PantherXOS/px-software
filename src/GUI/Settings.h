//
// Created by hamzeh on 12/15/19.
//

#ifndef PX_SOFTWARE_SETTINGS_H
#define PX_SOFTWARE_SETTINGS_H

#define VIEW_MESSAGE_FONT_SIZE          16
#define VIEW_LOADING_ICON_SIZE          128

#define TOP_MENU_BUTTON_SIZE            32
#define TOP_MENU_ICON_SIZE              24

#define SEARCH_BAR_H                    TOP_MENU_BUTTON_SIZE

#define CONTENT_LIST_ITEM_W             200
#define CONTENT_LIST_ULINE_W            CONTENT_LIST_ITEM_W-10
#define CONTENT_LIST_ULINE_H            2
#define CONTENT_LIST_ICON_SIZE          16
#define CONTENT_LIST_STYLESHEET         "background-color: transparent;"
#define CONTENT_LIST_TITLE_FONT_SIZE    12
#define CONTENT_LIST_SUBTITLE_FONT_SIZE 11
#define CONTENT_LIST_ITEM_RICON_SIZE    20

#define CONTENT_LIST_EMPTY_ITEM_W       CONTENT_LIST_ITEM_W-10
#define CONTENT_LIST_EMPTY_ITEM_H       35
#define CONTENT_LIST_EMPTY_LEFT_MARGIN      5
#define CONTENT_LIST_EMPTY_TOP_MARGIN       15
#define CONTENT_LIST_EMPTY_RIGHT_MARGIN     0
#define CONTENT_LIST_EMPTY_BOTTOM_MARGIN    5

#define CATEGORY_TITLE_FONT_SIZE        12
#define CATEGORY_DESC_FONT_SIZE         10
#define CATEGORY_ICON_CACHE_DIR         "/.cache/px/px-software/categories/"
#define CATEGORY_ICON_SIZE              48
#define CATEGORY_ITEM_HEIGHT            CATEGORY_ICON_SIZE + 12
#define CATEGORY_ITEM_WIDTH             300

#define PACKAGE_ICON_CACHE_DIR          "/.cache/px/px-software/images/"
#define PACKAGE_BUTTON_W                128
#define PACKAGE_BUTTON_H                28
#define PACKAGE_ICON_SIZE               96
#define PACKAGE_SCREENSHOTS_CACHE_DIR   "/.cache/px/px-software/images/"
#define PACKAGE_SCREENSHOT_W            640
#define PACKAGE_SCREENSHOT_H            480
#define PACKAGE_TITLE_FONT_SIZE         12
#define PACKAGE_DESC_FONT_SIZE          10
#define PACKAGE_INSTALL_STYLESHEET      "QPushButton {background-color: blue; color: white;}"
#define PACKAGE_UPDATE_STYLESHEET       "QPushButton {background-color: green; color: white;}"
#define PACKAGE_REMOVE_STYLESHEET       "QPushButton {background-color: red; color: white;}"
#define PACKAGE_UPTODATE_STYLESHEET     "QPushButton {background-color: gray; color: black;}"
#define PACKAGE_ICON_STYLESHEET         "QLabel {border 1px solid rgb(80, 80, 80);}"
#define PACKAGE_LICENSE_STYLESHEET      "QLabel { color : gray; }"
#define PACKAGE_SCREENSHOT_STYLESHEET   "background-color: transparent;"
#define PACKAGE_DETAILS_RIGHT_PANEL_MARGIN  7
#define PACKAGE_DETAILS_RIGHT_PANEL_SPACE   15

#define SCREENSHOT_ARROW_KEY_ICON_SIZE  128
#define SCREENSHOT_ARROW_KEY_STYLESHEET "QPushButton {border: 0px transparent;}"
#endif //PX_SOFTWARE_SETTINGS_H
