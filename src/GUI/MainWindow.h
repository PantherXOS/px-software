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

#include "PxQWidget.h"
#include "PxQListWidgetItem.h"
#include "ContentList.h"
#include "PackageManager.h"
#include "PackageListWidgetItem.h"
#include "PackageDetails.h"
#include "SearchPackagesList.h"
#include "PxSearchBar.h"
#include "PackageManagerTracker.h"
#include "Settings.h"
#include "ScreenshotItem.h"
#include "ScreenShotViewer.h"
#include "UserUpdateNotification.h"
#include "CacheManager.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QString dbPath, QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void mousePressEvent(QMouseEvent *event) override;
    void settingsButtonHandler();
    void backButtonHandler();
    void forwardButtonHandler();
    void helpButtonHandler();
    void leftPanelItemHandler(QListWidgetItem *item);
    void searchBoxHandler(const QString &);
    void screenshotItemClickedHandler(ScreenshotItem *item);
    void showTerminalSignalHandler(TerminalWidget *terminal);
    
private:
    bool            getFreeDiskSpace(QString path, QString &result);
    QWidget  *      createBottombar();
    void            reloadTopBar();
    void            loadWindow(int id);
    QToolBar *      loadTopMenu();
    void            refreshContentLayouts(QWidget *item);
    PxQScrollArea * dbErrorHandling();

    QWidget *window;
    QStackedWidget *contentLayouts;

    ContentList *contentList;
    QString viewName;
    QString packageName;
    QPushButton *settingsButton;
    QPushButton *backButton;
    QPushButton *forwardButton;
    PxSearchBar *addressBar;
    QPushButton *helpButton;
};

#endif //PX_SOFTWARE_MAINWINDOW_H
