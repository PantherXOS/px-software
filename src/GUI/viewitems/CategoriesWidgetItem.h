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
#include <QWidget>
#include <QString>
#include <QScrollArea>
#include <QDebug>

#include "PxQScrollArea.h"
#include "PxQListWidgetItem.h"
#include "CategoryWidget.h"
#include "Settings.h"

class CategoryWidgetScrollArea : public PxQScrollArea{
public:
    CategoryWidgetScrollArea(const QString &title, const QVector<Category *> categories, PxQScrollArea *parent = nullptr) : PxQScrollArea(title,parent){
        for (auto cat : categories) {
            auto catLayout = new CategoryWidget(cat);
            categoryWidgets.push_back(catLayout);
        }
    }

    bool event(QEvent * event) override{
        if(event->type() == QEvent::Resize) {
            auto layout = new QGridLayout;
            int i = 0;
            for (auto cat : categoryWidgets) {
                if(this->width() - CONTENT_LIST_ITEM_W > (3 * cat->size().width()))
                    layout->addWidget(cat, i/3, i%3);
                else if(this->width() - CONTENT_LIST_ITEM_W > (2 * cat->size().width()))
                    layout->addWidget(cat, i/2, i%2);
                else
                    layout->addWidget(cat, i, 1);
                i++;
            }

            auto widget=new PxQWidget;
            widget->setLayout(layout);
            layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            setWidget(widget);
        };
        return PxQScrollArea::event(event);
    }

private:
    QVector<CategoryWidget *> categoryWidgets;
};

class CategoriesWidgetItem : public PxQListWidgetItem{
public:
    CategoriesWidgetItem(const QString &id, const QString &title, const QFont &font, const QString &iconItemFile,
                         QListWidget *parent = nullptr) : PxQListWidgetItem(id, title, font, iconItemFile,
                                                                            parent) {
        this->id=id;
        this->title=title;
    }

    PxQScrollArea *getView() override{
        auto m_pkgMgrTrk = PackageManagerTracker::Instance();
        auto cats = m_pkgMgrTrk->categoryList();
        auto view = new CategoryWidgetScrollArea(title,cats);
        return view;
    }

private:
    QString id;
    QString title;
    PxQScrollArea *view;
};





#endif //PX_SOFTWARE_CATEGORIESWIDGETITEM_H
