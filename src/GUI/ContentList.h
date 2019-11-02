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

#include "PxQWidget.h"
#include "PxQListWidgetItem.h"
#include "DataAccessLayer.h"
#include "CategoryWidget.h"

enum CONTENT_SECTIONS{
    STORE_LATEST,
    STORE_RECOMMENDED,
    STORE_CATEGORIES,
    APPS_INSTALLED,
    APPS_UPDATES,
    SYSTEM_UPDATES
};


class ContentList {
public:
    ContentList(int w, int h);
    QListWidget *getItemList();
    PxQWidget *getItem(int id);
private:
    PxQListWidgetItem *createItem(QString title);
    PxQListWidgetItem *createSubItem(int contentId);
    QListWidgetItem   *createSeperator();

    QListWidget *itemList;
    map<int, PxQWidget*> widgetsMap;
};


#endif //PX_SOFTWARE_CONTENTLIST_H
