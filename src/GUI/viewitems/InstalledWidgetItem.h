//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_INSTALLEDWIDGETITEM_H
#define PX_SOFTWARE_INSTALLEDWIDGETITEM_H
#include "PxQScrollArea.h"
#include "InstalledPackageListView.h"
#include "PxQListWidgetItem.h"

class InstalledWidgetItem : public PxQListWidgetItem{
public:
    InstalledWidgetItem(const QString &title, const QFont &font, const QString &iconItemFile,
                        QListWidget *parent = nullptr) : PxQListWidgetItem(title, font, iconItemFile, parent) {
        InstalledPackageListView::init(title);
        view = InstalledPackageListView::Instance();
    }

    PxQScrollArea *getView() override{
        view->refresh();
        return view;
    }

private:
    InstalledPackageListView *view;
};
#endif //PX_SOFTWARE_INSTALLEDWIDGETITEM_H
