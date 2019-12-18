//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_RECOMMENDEDWIDGETITEM_H
#define PX_SOFTWARE_RECOMMENDEDWIDGETITEM_H
#include "PxQScrollArea.h"
#include "TagPackageList.h"
#include "PxQListWidgetItem.h"

class RecommendedWidgetItem : public PxQListWidgetItem{
public:
    RecommendedWidgetItem(const QString &title, const QFont &font, const QString &iconItemFile,
                          QListWidget *parent = nullptr) : PxQListWidgetItem(title, font, iconItemFile, parent) {
        view = new TagPackageList(title, "recommended");
    }

    PxQScrollArea *getView() override{
        return view;
    }

private:
    TagPackageList *view;
};
#endif //PX_SOFTWARE_RECOMMENDEDWIDGETITEM_H
