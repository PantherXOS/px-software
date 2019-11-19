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
#include "InProgressPakcageListView.h"

enum CONTENT_SECTIONS{
    STORE_LATEST,
    STORE_RECOMMENDED,
    STORE_CATEGORIES,
    APPS_INSTALLED,
    APPS_UPDATES,
    IN_PROGRESS,
    SYSTEM_UPDATES
};

using namespace PKG;
class ContentList : public QListWidget{
Q_OBJECT
public:
    ContentList(QListWidget *parent = 0);
    PxQScrollArea *getItem(int id);

private:
    PxQListWidgetItem *createItem(QString title);
    PxQListWidgetItem *createSubItem(int contentId);
    QListWidgetItem   *createSeperator();
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
};


#endif //PX_SOFTWARE_CONTENTLIST_H
