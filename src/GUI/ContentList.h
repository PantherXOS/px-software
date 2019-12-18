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

#include "src/GUI/px-qobjects/PxQScrollArea.h"
#include "src/GUI/px-qobjects/PxQListWidgetItem.h"
#include "CategoryWidget.h"
#include "PackageManager.h"
#include "PackageManagerTracker.h"
#include "src/GUI/viewitems/InstalledPackageListView.h"
#include "src/GUI/viewitems/UserUpdatablePackageListView.h"
#include "src/GUI/viewitems/SystemUpdatablePackageListView.h"
#include "src/GUI/viewitems/InProgressPackageListView.h"
#include "TagPackageList.h"
#include "Settings.h"
#include "src/GUI/viewitems/InProgressWidgetItem.h"
#include "src/GUI/viewitems/InstalledWidgetItem.h"
#include "src/GUI/viewitems/UserUpdatableWidgetItem.h"
#include "src/GUI/viewitems/SystemUpdatableWidgetItem.h"
#include "src/GUI/viewitems/LatestWidgetItem.h"
#include "src/GUI/viewitems/RecommendedWidgetItem.h"
#include "src/GUI/viewitems/CategoriesWidgetItem.h"
#include "src/GUI/viewitems/EmptyWidgetItem.h"

enum CONTENT_SECTIONS{
    FIRST_SEPERATOR,
    STORE_TITLE,
    STORE_LATEST,
    STORE_LATEST_LINE,
    STORE_RECOMMENDED,
    STORE_RECOMMENDED_LINE,
    STORE_CATEGORIES,
    STORE_CATEGORIES_LINE,
    SECOND_SEPERATOR,
    USER_APP_TITLE,
    APPS_INSTALLED,
    APPS_INSTALLED_LINE,
    APPS_UPDATES,
    APPS_UPDATES_LINE,
    IN_PROGRESS,
    IN_PROGRESS_LINE,
    THIRD_SEPERATOR,
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
    void createSeperator();

    InProgressWidgetItem *pInProgressWidgetItem;
    InstalledWidgetItem *pInstalledWidgetItem;
    UserUpdatableWidgetItem *pUpdatableWidgetItem;
    SystemUpdatableWidgetItem *pSystemUpdatableWidgetItem;
    LatestWidgetItem *pLatestWidgetItem;
    RecommendedWidgetItem *pRecommendedWidgetItem;
    CategoriesWidgetItem *pCategoriesWidgetItem;
    map<int,PxQScrollArea*> viewMap;
};


#endif //PX_SOFTWARE_CONTENTLIST_H
