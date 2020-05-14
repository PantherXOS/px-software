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

#ifndef PX_SOFTWARE_INSTALLEDPACKAGELISTVIEW_H
#define PX_SOFTWARE_INSTALLEDPACKAGELISTVIEW_H
#include <QVector>
#include <QMovie>

#include "PxQWidget.h"
#include "PxQScrollArea.h"
#include "PackageManager.h"
#include "PackageManagerTracker.h"
#include "PackageListWidgetItem.h"
#include "QProgressIndicator.h"
#include "Settings.h"

using namespace PKG;

class InstalledPackageListView : public PxQScrollArea{
    Q_OBJECT
public:
    static InstalledPackageListView *Instance();
    static void init(const QString &title);
    void refresh();

private slots:
    void getInstalledPackages(const QVector<Package *> &packageList);
    void packageProgressDoneHandler(const QString&);
    void taskFailedHandler(const QUuid &, const QString&);

private:
    InstalledPackageListView(const QString &title, PxQScrollArea *parent);
    static InstalledPackageListView *_instance;
    QBoxLayout *boxLayout=nullptr;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
    QUuid taskId;
};


#endif //PX_SOFTWARE_INSTALLEDPACKAGELISTVIEW_H
