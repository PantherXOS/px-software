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


#ifndef PX_SOFTWARE_PACKAGEMANAGER_H
#define PX_SOFTWARE_PACKAGEMANAGER_H

#include <QObject>
#include <QMap>
#include <QPointer>
#include <QUuid>
#include <QTimer>

#include "GuixWrapper.h"
#include "GUIX/GuixProfileStatusTask.h"
#include "DataAccessLayer.h"

class AsyncTaskRunner;

namespace PKG {
    class PackageManager : public QObject {
        Q_OBJECT
    private:
        explicit PackageManager(const QString &dbPath, QObject *parent = nullptr);

    protected slots:
        void refreshProfile(const std::function<void()> &callback = nullptr, bool force = false);

    protected:
        bool prepareAndExec(AsyncTaskRunner *worker, bool refresh = false);
        QUuid getProfileAndPerform(const std::function<void(const QUuid &, const GuixProfile &)> &task);

    protected:
//        static void ApplyProfileOnPackages(const GuixProfile &profile, QVector<Package *> &packageList);

    public:
        static bool Init(const QString &dbPath, QObject *parent = nullptr);
        static void Destruct();
        static PackageManager *Instance();

    public slots:
        QUuid requestInstalledPackages();
        QUuid requestUserUpgradablePackages();
        QUuid requestSystemUpgradablePackages();
        QUuid requestCategoryPackages(const QString &categoryName);
        QUuid requestPackageSearch(const QString &keyword);
        QUuid requestTagPackages(const QString &tagName);
        QUuid requestPackageDetails(const QString &packageName);
        QUuid requestPackageInstallation(const QString &packageName);
        QUuid requestPackageUpdate(const QStringList &packageNameList);
        QUuid requestPackageRemoval(const QString &packageName);
        QUuid requestDBPackageUpdate();
        
        bool requestTaskCancel(const QUuid &taskId);

    public:
        QVector<Category *> categoryList();
        bool isInited();
        
    signals:
        void installedPackagesReady(const QUuid &taskId, const QVector<Package *> &packageList);
        void userUpgradablePackagesReady(const QUuid &taskId, const QVector<Package *> &packageList);
        void systemUpgradablePackagesReady(const QUuid &taskId, const QVector<Package *> &packageList);
        void categoryPackagesReady(const QUuid &taskId, const QVector<Package *> &packageList);
        void packageSearchResultsReady(const QUuid &taskId, const QVector<Package *> &packageList);
        void tagPackagesReady(const QUuid &taskId, const QVector<Package *> &packageList);
        void packageDetailsReady(const QUuid &taskId, Package *package);
        void packageInstalled(const QUuid &taskId, const QString &name);
        void packageUpdated(const QUuid &taskId, const QStringList &nameList);
        void packageRemoved(const QUuid &taskId, const QString &name);

        void newTaskData(const QUuid &taskId, const QString &data);
        void taskDone(const QUuid &taskId, const QString &data);
        void taskFailed(const QUuid &taskId, const QString &message);
        void taskCanceled(const QUuid &taskId);

    private:
        static PackageManager *_instance;
        DataAccessLayer *m_db;
//        QMap<QUuid, QPointer<AsyncTaskRunner> > m_workerDict;
//        QMap<QUuid, QMetaObject::Connection> m_internalWorkersDict;
        GuixProfile m_profile;
        GuixWrapper *m_wrapper;
    };
}

#endif //PX_SOFTWARE_PACKAGEMANAGER_H
