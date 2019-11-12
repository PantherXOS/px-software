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

enum CONTENT_SECTIONS{
    STORE_LATEST,
    STORE_RECOMMENDED,
    STORE_CATEGORIES,
    APPS_INSTALLED,
    APPS_UPDATES,
    SYSTEM_UPDATES
};

using namespace PKG;
class ContentList : public QListWidget{
Q_OBJECT
public:
    ContentList(QListWidget *parent = 0);
    PxQScrollArea *getItem(int id);

private slots:
    void getInstalledPackages(const QVector<Package *> &packageList);

private:
    PxQListWidgetItem *createItem(QString title);
    PxQListWidgetItem *createSubItem(int contentId);
    QListWidgetItem   *createSeperator();
    PackageListWidget *installedPackageList;
    PackageManager *m_pkgMgr = nullptr;
};


#endif //PX_SOFTWARE_CONTENTLIST_H
