//
// Created by hamzeh on 12/18/19.
//

#ifndef PX_SOFTWARE_EMPTYWIDGETITEM_H
#define PX_SOFTWARE_EMPTYWIDGETITEM_H
#include "PxQScrollArea.h"
#include "InProgressPackageListView.h"
#include "PxQListWidgetItem.h"

// TODO When I can to click on title item in the left panel and find out that
//  this item is non-selectable, This class should be removed.
class EmptyWidgetItem : public PxQListWidgetItem{
public:
    EmptyWidgetItem(QListWidget *parent = nullptr) : PxQListWidgetItem("", QFont(), "", parent) {
    }

    PxQScrollArea *getView() override{
        return nullptr;
    }
};
#endif //PX_SOFTWARE_EMPTYWIDGETITEM_H
