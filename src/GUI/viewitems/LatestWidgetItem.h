//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_LATESTWIDGETITEM_H
#define PX_SOFTWARE_LATESTWIDGETITEM_H
#include "PxQScrollArea.h"
#include "TagPackageList.h"
#include "PxQListWidgetItem.h"

class LatestWidgetItem : public PxQListWidgetItem{
public:
    LatestWidgetItem(const QString &id, const QString &title, const QFont &font, const QString &iconItemFile,
                     QListWidget *parent = nullptr) : PxQListWidgetItem(title, title, font, iconItemFile,
                                                                        parent) {
        this->id = id;
        this->title = title;
    }

    PxQScrollArea *getView() override{
        view = new TagPackageList(title, "latest");
        return view;
    }

private:
    TagPackageList *view;
    QString id;
    QString title;
};
#endif //PX_SOFTWARE_LATESTWIDGETITEM_H
