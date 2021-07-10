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

#include "PXContentWidget.h"
#include "PackageManager.h"
#include "PackageManagerTracker.h"
#include "Settings.h"
#include <QBoxLayout>

using namespace PKG;

#ifndef PX_SOFTWARE_INPROGRESSPAKCAGELISTVIEW_H
#define PX_SOFTWARE_INPROGRESSPAKCAGELISTVIEW_H


class InProgressPackageListView : public PXContentWidget{
Q_OBJECT
public:
    static InProgressPackageListView *Instance();
    static void init(const QString &title);

private slots:
    void packageProgressDoneHandler(const QString&);
    void packageProgressDoneHandler(const QString&, const QString&);
    void refresh();

private:
    InProgressPackageListView(const QString &title, PXContentWidget *parent);
    static InProgressPackageListView *_instance;
    QBoxLayout *boxLayout=nullptr;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
};
#endif //PX_SOFTWARE_INPROGRESSPAKCAGELISTVIEW_H
