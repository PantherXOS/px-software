//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_INPROGRESSWIDGETITEM_H
#define PX_SOFTWARE_INPROGRESSWIDGETITEM_H
#include "PxQScrollArea.h"
#include "InProgressPackageListView.h"
#include "PxQListWidgetItem.h"

class InProgressWidgetItem : public PxQListWidgetItem{
public:
    InProgressWidgetItem(const QString &title, const QFont &font, const QString &iconItemFile,
                         QListWidget *parent = nullptr) : PxQListWidgetItem(title, font, iconItemFile, parent) {
        InProgressPackageListView::init(title);
        view = InProgressPackageListView::Instance();
        view->refresh();
    }

    PxQScrollArea *getView() override{
        return view;
    }

    void refresh(){
        view->refresh();
    }

private:
    InProgressPackageListView *view;
};
#endif //PX_SOFTWARE_INPROGRESSWIDGETITEM_H
