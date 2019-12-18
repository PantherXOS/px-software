//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_INSTALLEDWIDGETITEM_H
#define PX_SOFTWARE_INSTALLEDWIDGETITEM_H
#include "src/GUI/px-qobjects/PxQScrollArea.h"
#include "InstalledPackageListView.h"
#include "src/GUI/px-qobjects/PxQListWidgetItem.h"

class InstalledWidgetItem : public PxQListWidgetItem{
public:
    InstalledWidgetItem(const QString &title, const QFont &font, const QString &iconItemFile,
                        QListWidget *parent = nullptr) : PxQListWidgetItem(title, font, iconItemFile, parent) {
        InstalledPackageListView::init(title);
        view = InstalledPackageListView::Instance();
        view->refresh();
    }

    PxQScrollArea *getView() override{
        view->refresh();
        return view;
    }

private:
    InstalledPackageListView *view;
};
#endif //PX_SOFTWARE_INSTALLEDWIDGETITEM_H
