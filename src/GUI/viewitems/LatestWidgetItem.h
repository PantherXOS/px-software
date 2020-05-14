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

#ifndef PX_SOFTWARE_LATESTWIDGETITEM_H
#define PX_SOFTWARE_LATESTWIDGETITEM_H
#include "PxQScrollArea.h"
#include "TagPackageList.h"
#include "PxQListWidgetItem.h"

class LatestWidgetItem : public PxQListWidgetItem{
public:
    LatestWidgetItem(const QString &id, const QString &title, const QFont &font, const QString &iconItemFile,
                     QListWidget *parent = nullptr) : PxQListWidgetItem(title, title, font, iconItemFile,
                                                                        parent) {
        this->id = id;
        this->title = title;
    }

    PxQScrollArea *getView() override{
        view = new TagPackageList(title, "latest");
        return view;
    }

private:
    TagPackageList *view;
    QString id;
    QString title;
};
#endif //PX_SOFTWARE_LATESTWIDGETITEM_H
