//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_SYSTEMUPDATABLEWIDGETITEM_H
#define PX_SOFTWARE_SYSTEMUPDATABLEWIDGETITEM_H
#include "PxQScrollArea.h"
#include "SystemUpdatablePackageListView.h"
#include "PxQListWidgetItem.h"

class SystemUpdatableWidgetItem : public PxQListWidgetItem{
public:
    SystemUpdatableWidgetItem(const QString &title, const QFont &font, const QString &iconItemFile,
                              QListWidget *parent = nullptr) : PxQListWidgetItem(title, font, iconItemFile, parent) {
        SystemUpdatablePackageListView::init(title);
        view = SystemUpdatablePackageListView::Instance();
        view->refresh();
    }

    PxQScrollArea *getView() override{
        view->refresh();
        return view;
    }

private:
    SystemUpdatablePackageListView *view;
};
#endif //PX_SOFTWARE_SYSTEMUPDATABLEWIDGETITEM_H
