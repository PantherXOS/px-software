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

#ifndef PX_SOFTWARE_USERUPDATENOTIFICATION_H
#define PX_SOFTWARE_USERUPDATENOTIFICATION_H
#include <QObject>
#include "PackageManagerTracker.h"
#include "src/GUI/notification/pxnotification.h"

class UserUpdateNotification : public QObject {
    Q_OBJECT

public:
    static UserUpdateNotification &instance(){
        static UserUpdateNotification _instance;
        return _instance;
    }

private slots:
    void userUpdatablePackageListHandler(const QUuid &, const QVector<Package *> packageList) {
        if(packageList.size())
            Notification::notify("(" + QString::number(packageList.size()) + ") "+ tr("User Upgradable Packages are available."));
        disconnect(userUpdatablePackageSignalConnection);
    };


    void systemUpdatablePackageListHandler(const QUuid &, const QVector<Package *> packageList) {
        if(packageList.size())
            Notification::notify("(" + QString::number(packageList.size()) + ") "+ tr("System Upgradable Packages are available."));
        disconnect(systemUpdatablePackageSignalConnection);
    };

private:
    UserUpdateNotification(){
        m_pkgMgr = PKG::PackageManager::Instance();
        userUpdatablePackageSignalConnection = connect(m_pkgMgr, SIGNAL(userUpgradablePackagesReady(
                                         const QUuid &,
                                         const QVector<Package *>)), this, SLOT(userUpdatablePackageListHandler(
                                                                                        const QUuid &, const QVector<Package *>)));

        systemUpdatablePackageSignalConnection = connect(m_pkgMgr, SIGNAL(systemUpgradablePackagesReady(
                                                        const QUuid &,
                                                        const QVector<Package *>)), this, SLOT(systemUpdatablePackageListHandler(
                                                                                                       const QUuid &, const QVector<Package *>)));
    }

    PackageManager *m_pkgMgr = nullptr;
    QMetaObject::Connection userUpdatablePackageSignalConnection;
    QMetaObject::Connection systemUpdatablePackageSignalConnection;

};
#endif //PX_SOFTWARE_USERUPDATENOTIFICATION_H
