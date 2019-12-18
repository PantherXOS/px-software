//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_USERUPDATABLEWIDGETITEM_H
#define PX_SOFTWARE_USERUPDATABLEWIDGETITEM_H
#include "src/GUI/px-qobjects/PxQScrollArea.h"
#include "UserUpdatablePackageListView.h"
#include "src/GUI/px-qobjects/PxQListWidgetItem.h"

class UserUpdatableWidgetItem : public PxQListWidgetItem{
public:
    UserUpdatableWidgetItem(const QString &title, const QFont &font, const QString &iconItemFile,
                            QListWidget *parent = nullptr) : PxQListWidgetItem(title, font, iconItemFile, parent) {
        UserUpdatablePackageListView::init(title);
        view = UserUpdatablePackageListView::Instance();
        view->refresh();
    }

    PxQScrollArea *getView() override{
        view->refresh();
        return view;
    }

private:
    UserUpdatablePackageListView *view;
};

#endif //PX_SOFTWARE_USERUPDATABLEWIDGETITEM_H
