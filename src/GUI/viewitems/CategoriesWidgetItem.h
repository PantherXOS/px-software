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

#ifndef PX_SOFTWARE_CATEGORIESWIDGETITEM_H
#define PX_SOFTWARE_CATEGORIESWIDGETITEM_H
#include "PxQScrollArea.h"
#include "PxQListWidgetItem.h"
#include "CategoryWidget.h"

class CategoriesWidgetItem : public PxQListWidgetItem{
public:
    CategoriesWidgetItem(const QString &id, const QString &title, const QFont &font, const QString &iconItemFile,
                         QListWidget *parent = nullptr) : PxQListWidgetItem(id, title, font, iconItemFile,
                                                                            parent) {
        this->id=id;
        this->title=title;
    }

    PxQScrollArea *getView() override{
        auto layout = new QGridLayout;
        auto m_pkgMgrTrk = PackageManagerTracker::Instance();
        auto cats = m_pkgMgrTrk->categoryList();
        int i = 0;
        for (auto cat : cats) {
            auto catLayout = new CategoryWidget(cat);
            layout->addWidget(catLayout, i/3, i%3);
            i++;
        }

        auto widget=new PxQWidget;
        widget->setLayout(layout);
        layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        view = new PxQScrollArea(title);
        view->setWidget(widget);

        return view;
    }

private:
    QString id;
    QString title;
    PxQScrollArea *view;
};
#endif //PX_SOFTWARE_CATEGORIESWIDGETITEM_H
