//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_USERUPDATABLEWIDGETITEM_H
#define PX_SOFTWARE_USERUPDATABLEWIDGETITEM_H
#include "PxQScrollArea.h"
#include "UserUpdatablePackageListView.h"
#include "PxQListWidgetItem.h"

class UserUpdatableWidgetItem : public PxQListWidgetItem{
public:
    UserUpdatableWidgetItem(const QString &title, const QFont &font, const QString &iconItemFile,
                            QListWidget *parent = nullptr) : PxQListWidgetItem(title, font, iconItemFile, parent) {
        UserUpdatablePackageListView::init(title);
        view = UserUpdatablePackageListView::Instance();
        view->refresh();
    }

    PxQScrollArea *getView() override{
        return view;
    }

    void refresh(){
        view->refresh();
    }

private:
    UserUpdatablePackageListView *view;
};

#endif //PX_SOFTWARE_USERUPDATABLEWIDGETITEM_H
