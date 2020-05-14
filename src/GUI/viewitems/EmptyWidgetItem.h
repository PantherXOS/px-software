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

#ifndef PX_SOFTWARE_EMPTYWIDGETITEM_H
#define PX_SOFTWARE_EMPTYWIDGETITEM_H
#include <QScrollArea>

#include "PxQListWidgetItem.h"
#include "PxQScrollArea.h"

// TODO When I can to click on title item in the left panel and find out that
//  this item is non-selectable, This class should be removed.
class EmptyWidgetItem : public PxQListWidgetItem{
public:
    EmptyWidgetItem(QListWidget *parent = nullptr) : PxQListWidgetItem("", "", QFont(), "", parent) {
    }

    PxQScrollArea *getView() override {
        return nullptr;
    }
};
#endif //PX_SOFTWARE_EMPTYWIDGETITEM_H
