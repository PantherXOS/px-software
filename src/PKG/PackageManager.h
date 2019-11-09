//
// Created by Reza Alizadeh Majd on 10/21/19.
//

#ifndef PX_SOFTWARE_PACKAGEMANAGER_H
#define PX_SOFTWARE_PACKAGEMANAGER_H

#include <QObject>
#include <QMap>
#include <QPointer>
#include "DataAccessLayer.h"
#include "GuixParser.h"
#include "../AsyncTaskRunner.h"

namespace PKG {
    class PackageManager : public QObject {
        Q_OBJECT
    public:
        explicit PackageManager(const QString &dbPath, QObject *parent = nullptr);

    protected:
        QPointer<AsyncTaskRunner> initWorker();
        void removeWorker(const QUuid &id);

    public slots:
        QUuid requestInstalledPackages();
        QUuid requestUserUpgradablePackages();
        QUuid requestSystemUpgradablePackages();
        QUuid requestPackageInstallation(const QString &packageName);
        QUuid requestPackageUpdate(const QString &packageName);

    signals:
        void failed(const QString &message);
        void installedPackagesReady(const QVector<Package *> &packageList);
        void userUpgradablePackagesReady(const QVector<Package *> &packageList);
        void systemUpgradablePackagesReady(const QVector<Package *> &packageList);
        void packageInstalled(const QString &name);
        void packageUpdated(const QString &name);
        void newTaskData(const QUuid &taskId, const QString &data);
        void taskDone(const QUuid &taskId, const QString &data);
        void taskFailed(const QUuid &taskId, const QString &message);

    private:
        DataAccessLayer *m_db;
        GuixParser *m_parser;
        QMap<QUuid, QPointer<AsyncTaskRunner> > m_workerDict;
    };
}

#endif //PX_SOFTWARE_PACKAGEMANAGER_H
