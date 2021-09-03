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

#ifndef PX_SOFTWARE_PACKAGEMANAGERTRACKER_H
#define PX_SOFTWARE_PACKAGEMANAGERTRACKER_H

#include <iostream>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QObject>
#include <QMap>

#include "PackageManager.h"
#include "DataEntities.h"

using namespace PKG;
using namespace std;
class PackageManagerTracker : public QObject{
Q_OBJECT
public:
    enum PackageStatus{
        NOTHING,
        INSTALLING,
        REMOVING,
        UPDATING,
    };

    class InProgressPackage{
    public:
        QString name;
        PackageStatus status;
    };

    QVector<Category *> categoryList();
    QVector<Package *> inProgressList();
    QUuid requestInstalledPackageList();
    QUuid requestUserUpdatablePackageList();
    QUuid requestSystemUpdatablePackageList();
    QUuid requestSystemUpdate();

    static void init(const QString &title);
    static PackageManagerTracker *Instance();
    bool requestPackageInstallation(const QString &packageName);
    bool requestPackageUpdate(const QString &packageName);
    bool requestPackageRemoval(const QString &packageName);
    bool requestPackageTaskCancel(const QString &packageName);
    void requestTaskCancel(const QUuid &taskID);
    bool packageInProgress(const QString &packageName);
    bool inInstalling(const QString &packageName);
    bool inRemoving(const QString &packageName);
    bool inUpdating(const QString &packageName);

private slots:
    void installedPackageListHandler(const QUuid &taskId, const QVector<Package *> &packageList);
    void userUpdatablePackageListHandler(const QUuid &taskId, const QVector<Package *> &packageList);
    void systemUpdatablePackageListHandler(const QUuid &taskId, const QVector<Package *> &packageList);
    void packageInstalledHandler(const QUuid &taskId,const QString &name);
    void packageRemovedHandler(const QUuid &taskId,const QString &name);
    void packageUpdatedHandler(const QUuid &taskId,const QStringList &nameList);
    void packageTaskCanceledHandler(const QUuid &taskId);
    void taskFailedHandler(const QUuid &, const QString &message);
    void taskDataHandler(const QUuid &taskId, const QString &data);
    void taskDoneHandler(const QUuid &, const QString &message);

signals:
    void installedPackageListReady(const QVector<Package *> &packageList);
    void userUpdatablePackageListReady(const QVector<Package *> &packageList);
    void systemUpdatablePackageListReady(const QVector<Package *> &packageList);
    void packageInstalled(const QString &name);
    void packageUpdated(const QString &name);
    void packageRemoved(const QString &name);
    void packageTaskCanceled(const QString &name);
    void progressFailed(const QString &name, const QString &message);
    void taskDataReceivedWithUuid(const QUuid   &uuid, const QString &data);
    void taskDataReceived(const QString &name, const QString &data);
    void taskFailed(const QUuid &, const QString &message);
    void taskDone(const QUuid &taskId, const QString &data);
    void inProgressRequest(void);

private:
    bool packageInProgress(const QString &packageName, QUuid &taskId);
    bool packageInProgress(const QUuid &taskId);
    PackageManagerTracker();
    static PackageManagerTracker *_instance;
    static QString _dbPath;
    PackageManager *m_pkgMgr = nullptr;
    map<QUuid , InProgressPackage> inProgressPackagesMap;
};


#endif //PX_SOFTWARE_PACKAGEMANAGERTRACKER_H
