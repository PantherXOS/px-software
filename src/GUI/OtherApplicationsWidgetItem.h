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

#ifndef PX_SOFTWARE_OTHERAPPLICATIONWIDGETITEM_H
#define PX_SOFTWARE_OTHERAPPLICATIONWIDGETITEM_H

#include <iostream>
#include <string>
#include <QWidget>
#include <QString>
#include <vector>
#include <QLabel>
#include <QImage>
#include <QIcon>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <QLineEdit>
#include <QStyle>
#include <QPainter>
#include <QStyleOption>
#include <QPalette>
#include <QColor>
#include <QListWidgetItem>

#include "DataEntities.h"
#include "FileDownloader.h"
#include "PackageManager.h"
#include "PackageManagerTracker.h"
#include "TerminalWidget.h"

using namespace std;
using namespace PKG;
class OtherApplicationsWidgetItem :public QWidget {
    Q_OBJECT
public:
    OtherApplicationsWidgetItem(QWidget *parent = nullptr);
};


class OtherApplicationsWidgetItem1 :public QListWidgetItem {
public:
    OtherApplicationsWidgetItem1(QWidget *parent){
        _widget = new OtherApplicationsWidgetItem(parent);
        setFlags(flags() & ~Qt::ItemIsSelectable);
    }

    OtherApplicationsWidgetItem *widget() {
        return _widget;
    }

private:
    OtherApplicationsWidgetItem *_widget;
};

#endif //PX_SOFTWARE_OTHERAPPLICATIONWIDGETITEM_H
