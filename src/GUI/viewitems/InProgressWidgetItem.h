/*
 * px-software - Qt-GUI package manager for guix on PantherX OS
 * Copyright (C) 2019, 2020  Hamzeh Nasajpour <h.nasajpour@pantherx.org>
 * Copyright (C) 2019, 2020  Reza Alizadeh Majd <r.majd@pantherx.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef PX_SOFTWARE_INPROGRESSWIDGETITEM_H
#define PX_SOFTWARE_INPROGRESSWIDGETITEM_H
#include "PxQScrollArea.h"
#include "InProgressPackageListView.h"
#include "PxQListWidgetItem.h"

class InProgressWidgetItem : public PxQListWidgetItem{
public:
    InProgressWidgetItem(const QString &id, const QString &title, const QFont &font, const QString &iconItemFile,
                         QListWidget *parent = nullptr) : PxQListWidgetItem(title, title, font, iconItemFile,
                                                                            parent) {
        buildRightLayout();
        InProgressPackageListView::init(title);
        view = InProgressPackageListView::Instance();
        this->setHidden(true);
    }

    void buildRightLayout(){
        auto rightIconLabel = new QLabel;
        QIcon ricon;
        QImage rimage(":images/general/src/GUI/resources/red");
        ricon.addPixmap(QPixmap::fromImage(rimage), QIcon::Normal, QIcon::On);
        QPixmap rpixmap = ricon.pixmap(QSize(CONTENT_LIST_ITEM_RICON_SIZE, CONTENT_LIST_ITEM_RICON_SIZE), QIcon::Normal, QIcon::On);
        rightIconLabel->setPixmap(rpixmap);
        rightLayout()->addWidget(rightIconLabel);
    }

    PxQScrollArea *getView() override{
        view->refresh();
        return view;
    }

private:
    InProgressPackageListView *view;
};
#endif //PX_SOFTWARE_INPROGRESSWIDGETITEM_H
