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

#ifndef PX_SOFTWARE_SYSTEMUPDATABLEPACKAGELISTVIEW_H
#define PX_SOFTWARE_SYSTEMUPDATABLEPACKAGELISTVIEW_H
#include <QVector>
#include <QMovie>

#include "PackageListWidget.h"
#include "PackageManagerTracker.h"

using namespace PKG;
class SystemUpdatablePackageListView : public PackageListWidget{
    Q_OBJECT
public:
    static SystemUpdatablePackageListView *Instance();
    static void init(const QString &title);
    void refresh();

private slots:
    void getSystemUpdatablePackages(const QVector<Package *> &packageList);
    void taskFailedHandler(const QUuid &, const QString&);

private:
    SystemUpdatablePackageListView(const QString &title, PXContentWidget *parent = nullptr);
    static SystemUpdatablePackageListView *_instance;
    QUuid taskId;
};



#endif //PX_SOFTWARE_SYSTEMUPDATABLEPACKAGELISTVIEW_H
