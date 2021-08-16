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

#ifndef PX_SOFTWARE_UPDATE_ALL_PACKAGE_ITEM_H
#define PX_SOFTWARE_UPDATE_ALL_PACKAGE_ITEM_H

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
#include "PackageManager.h"
#include "PackageManagerTracker.h"
#include "TerminalWidget.h"

using namespace std;
using namespace PKG;
class UpdateAllPackagesItem_widget :public QWidget {
    Q_OBJECT
public:
    UpdateAllPackagesItem_widget(bool system, const QVector<Package *> &list, QWidget *parent = nullptr);
private slots:
    void checkUserPackageList(const QString &name);

private:
    void refreshUpdateButtonStatus();
    QLabel      *_processLabel;
    QMovie      *_movie;
    QPushButton *_button;
    bool         _isUpdating = false;
    QUuid        _updatingAllTaskId;
    bool         _systemPackages = false;
    QVector<Package *> _packageList;
    PackageManagerTracker *_pkgMgrTrk;
    QVector<QString> _updatingPackages;
};


class UpdateAllPackagesItem :public QListWidgetItem {
public:
    UpdateAllPackagesItem(bool system, const QVector<Package *> & packages, QWidget *parent){
        _widget = new UpdateAllPackagesItem_widget(system, packages);
        setFlags(Qt::NoItemFlags);
    }

    UpdateAllPackagesItem_widget *widget() {
        return _widget;
    }

private:
    UpdateAllPackagesItem_widget *_widget;
};

#endif //PX_SOFTWARE_UPDATE_ALL_PACKAGE_ITEM_H
