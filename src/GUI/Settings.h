//
// Created by hamzeh on 12/15/19.
//

#ifndef PX_SOFTWARE_SETTINGS_H
#define PX_SOFTWARE_SETTINGS_H


#define BOTTOMBAR_FONT_SIZE             12
#define BOTTOMBAR_ICON_SIZE             20

#define VIEW_MESSAGE_FONT_SIZE          16
#define VIEW_LOADING_ICON_SIZE          128

#define TOP_MENU_BUTTON_SIZE            36
#define TOP_MENU_ICON_SIZE              20

#define CONTENT_SCROLLAREA_STYLE        "QScrollArea {background-color: white; border: 0px; color: black;}"
#define CONTENT_WIDGET_STYLE            "QWidget {background-color: white; color: black;}"

#define SEARCH_BAR_H                    TOP_MENU_BUTTON_SIZE
#define SEARCH_BAR_FONT_NAME            "default"
#define SEARCH_BAR_FONT_SIZE            12
#define SEARCH_BAR_FONT_COLOR           "#474646"
#define SEARCH_BAR_BG_COLOR             "transparent"
#define SEARCH_BAR_STYLE                "QLineEdit {}"

#define CONTENT_LIST_ITEM_W             250
#define CONTENT_LIST_ULINE_W            CONTENT_LIST_ITEM_W-10
#define CONTENT_LIST_ULINE_H            1
#define CONTENT_LIST_ICON_SIZE          20
#define CONTENT_LIST_STYLESHEET         "QListWidget {background-color: transparent; border: 0px}"
#define CONTENT_LIST_TITLE_FONT_SIZE    12
#define CONTENT_LIST_SUBTITLE_FONT_SIZE 11
#define CONTENT_LIST_ITEM_RICON_SIZE    CONTENT_LIST_ICON_SIZE

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
#define PACKAGE_BUTTON_W                172
#define PACKAGE_BUTTON_H                32
#define PACKAGE_ICON_SIZE               64
#define PACKAGE_SCREENSHOTS_CACHE_DIR   "/.cache/px/px-software/images/"
#define PACKAGE_SCREENSHOT_W            640
#define PACKAGE_SCREENSHOT_H            480
#define PACKAGE_TITLE_FONT_SIZE         12
#define PACKAGE_DESC_FONT_SIZE          10
#define PACKAGE_INSTALL_STYLESHEET      "QPushButton {background-color: #36AC16; border: 0px; color: white;}"
#define PACKAGE_UPDATE_STYLESHEET       "QPushButton {background-color: #4B85E4; border: 0px; color: white;}"
#define PACKAGE_REMOVE_STYLESHEET       "QPushButton {background-color: #E44B4B; border: 0px; color: white;}"
#define PACKAGE_UPTODATE_STYLESHEET     "QPushButton {background-color: #7C7C7C; border: 0px; color: white;}"
#define PACKAGE_INPROGRESS_STYLESHEET   "QPushButton {background-color: #E1E0E0; border: 0px; color: black;}"
#define PACKAGE_ICON_STYLESHEET         "QLabel {border 1px solid rgb(80, 80, 80);}"
#define PACKAGE_LICENSE_STYLESHEET      "QLabel { color : gray; }"
#define PACKAGE_SCREENSHOT_STYLESHEET   "background-color: transparent;"
#define PACKAGE_DETAILS_RIGHT_PANEL_MARGIN  7
#define PACKAGE_DETAILS_RIGHT_PANEL_SPACE   15

#define SCREENSHOT_ARROW_KEY_ICON_SIZE  128
#define SCREENSHOT_ARROW_KEY_STYLESHEET "QPushButton {border: 0px transparent;}"

#define PACKAGE_STOP_PROGRESS_ICON_SIZE 20
#endif //PX_SOFTWARE_SETTINGS_H
