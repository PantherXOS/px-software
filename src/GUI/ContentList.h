//
// Created by hamzeh on 10/28/19.
//

#ifndef PX_SOFTWARE_CONTENTLIST_H
#define PX_SOFTWARE_CONTENTLIST_H

#include <QListWidget>
#include <QListWidgetItem>
#include <map>
#include <string>
#include <QWidget>
#include <QString>

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
    SYSTEM_UPDATES_LINE
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

private:
    void createTitle(QString title);
    PxQListWidgetItem * createSubItem(int contentId);

    InProgressWidgetItem *pInProgressWidgetItem;
    InstalledWidgetItem *pInstalledWidgetItem;
    UserUpdatableWidgetItem *pUserUpdatableWidgetItem;
    SystemUpdatableWidgetItem *pSystemUpdatableWidgetItem;
    LatestWidgetItem *pLatestWidgetItem;
    RecommendedWidgetItem *pRecommendedWidgetItem;
    CategoriesWidgetItem *pCategoriesWidgetItem;
    map<int,PxQScrollArea*> viewMap;
    QMap<int,QString> contentTitleMap;
};


#endif //PX_SOFTWARE_CONTENTLIST_H
