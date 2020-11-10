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

#ifndef PX_SOFTWARE_CONTENTLIST_H
#define PX_SOFTWARE_CONTENTLIST_H

#include <QListWidget>
#include <QListWidgetItem>
#include <map>
#include <string>
#include <QWidget>
#include <QString>
#include <QtGui/QGuiApplication>

#include "PxQScrollArea.h"
#include "PxQListWidgetItem.h"
#include "CategoryWidget.h"
#include "PackageManager.h"
#include "PackageManagerTracker.h"
#include "InstalledPackageListView.h"
#include "UserUpdatablePackageListView.h"
#include "SystemUpdatablePackageListView.h"
#include "InProgressPackageListView.h"
#include "TagPackageList.h"
#include "Settings.h"
#include "InProgressWidgetItem.h"
#include "InstalledWidgetItem.h"
#include "UserUpdatableWidgetItem.h"
#include "SystemUpdatableWidgetItem.h"
#include "LatestWidgetItem.h"
#include "RecommendedWidgetItem.h"
#include "CategoriesWidgetItem.h"
#include "EmptyWidgetItem.h"

enum CONTENT_SECTIONS{
    STORE_TITLE,
    STORE_LATEST,
    STORE_LATEST_LINE,
    STORE_RECOMMENDED,
    STORE_RECOMMENDED_LINE,
    STORE_CATEGORIES,
    STORE_CATEGORIES_LINE,
    USER_APP_TITLE,
    APPS_INSTALLED,
    APPS_INSTALLED_LINE,
    APPS_UPDATES,
    APPS_UPDATES_LINE,
    IN_PROGRESS,
    IN_PROGRESS_LINE,
    SYS_APP_TITLE,
    SYSTEM_UPDATES,
    SYSTEM_UPDATES_LINE,
    ERROR_PAGE,
};

using namespace PKG;
class ContentList : public QListWidget{
Q_OBJECT
public:
    ContentList(QListWidget *parent = nullptr);
    PxQScrollArea *getItem(int id);
    void setSelectedItem(QString name);

private slots:
    void getUserUpdatablePackages(const QVector<Package *> &packageList);
    void getSystemUpdatablePackages(const QVector<Package *> &packageList);
    void inProgressListUpdated();

private:
    void createTitle(QString title);
    PxQListWidgetItem * createSubItem(int contentId);

    EmptyWidgetItem *inProgressUline;
    InProgressWidgetItem *pInProgressWidgetItem;
    InstalledWidgetItem *pInstalledWidgetItem;
    UserUpdatableWidgetItem *pUserUpdatableWidgetItem;
    SystemUpdatableWidgetItem *pSystemUpdatableWidgetItem;
    LatestWidgetItem *pLatestWidgetItem;
    RecommendedWidgetItem *pRecommendedWidgetItem;
    CategoriesWidgetItem *pCategoriesWidgetItem;
    map<int,PxQScrollArea*> viewMap;
    QMap<int,QString> contentTitleMap;
    QMap<int,QString> contentIDMap;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
};


#endif //PX_SOFTWARE_CONTENTLIST_H
