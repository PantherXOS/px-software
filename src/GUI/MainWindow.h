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

#ifndef PX_SOFTWARE_MAINWINDOW_H
#define PX_SOFTWARE_MAINWINDOW_H

#include <sys/statvfs.h>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QListWidget>
#include <QStringList>
#include <QStackedLayout>
#include <QLineEdit>
#include <QTimerEvent>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QToolBar>
#include <QCloseEvent>

#include "PackageManager.h"
#include "PackageDetails.h"
#include "SearchPackagesList.h"
#include "PackageManagerTracker.h"
#include "Settings.h"
#include "ScreenshotItem.h"
#include "ScreenShotViewer.h"
#include "UserUpdateNotification.h"
#include "CacheManager.h"

#include "PXMainWindow.h"
#include "PXSideBarItem.h"
#include "CategoryView.h"
#include "TagPackageList.h"
#include "UpdatesItem.h"
#include "PackageListWidget.h"

#define  APPLIST_ARG_TILTE  "list"
#define  APP_ARG_TITLE      "app"

#define LATEST_APPS_TITLE       "Latest"
#define RECOMENDDED_APPS_TITLE  "Recommended"
#define CATEGORIES_ITEM_TITLE   "Categories"
#define INSTALLED_APPS_TITLE    "Installed"
#define USER_UPDATES_TITLE      "Updates"
#define IN_PROGRESS_APPS_TITLE  "In Progress"
#define SYSTEM_UPDATES_TITLE    "Updates"

#define LATEST_APPS_TAG         "latest"
#define RECOMENDDED_APPS_TAG    "recommended"
#define CATEGORIES_ITEM_TAG     "categories"
#define INSTALLED_APPS_TAG      "installed"
#define USER_UPDATES_TAG        "user_updates"
#define IN_PROGRESS_APPS_TAG    "in_progress"
#define SYSTEM_UPDATES_TAG      "system_updates"
class MainWindow : public PXMainWindow {
Q_OBJECT
public:
    explicit MainWindow(const QMap<QString, QString> &urlArgs, const QString &dbPath, QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void mousePressEvent(QMouseEvent *event);
    void getUserUpdatablePackages(const QVector<Package *> &packageList);
    void getSystemUpdatablePackages(const QVector<Package *> &packageList);
    void showTerminalSignalHandler(TerminalWidget *terminal);
    void screenshotItemClickedHandler(ScreenshotItem *item);
    void inProgressListUpdated();
    void closeEvent (QCloseEvent *event);

private:
    void buildSidebar(const QString &list);
    PXContentWidget *dbUpdatingView();
    PXContentWidget *dbUpdatingErrorView();
    void searchBoxTextEditedHandler(PXContentWidget *currentWidget, const QString&) override;
    void settingsButtonHandler() override;

    UpdatesItem *userUpdatesItem, *sysUpdatesItem;
    PXSideBarItem *inProgressItem;
    PackageManager          *m_pkgMgr = nullptr;
    PackageManagerTracker   *m_pkgMgrTrkr = nullptr;
    ScreenShotViewer *screenShotViewer = nullptr;
};

#endif //PX_SOFTWARE_MAINWINDOW_H
