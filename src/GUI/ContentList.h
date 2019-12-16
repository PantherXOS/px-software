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
    ContentList(QListWidget *parent = 0);
    PxQScrollArea *getItem(int id);
    void setSelectedItem(QString name);

private:
    void createItem(QString title);
    void createSubItem(int contentId);
    void createSeperator();
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
};


#endif //PX_SOFTWARE_CONTENTLIST_H
