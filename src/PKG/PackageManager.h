//
// Created by Reza Alizadeh Majd on 10/21/19.
//

#ifndef PX_SOFTWARE_PACKAGEMANAGER_H
#define PX_SOFTWARE_PACKAGEMANAGER_H

#include <QObject>
#include <QMap>
#include <QPointer>
#include <QUuid>
#include "DataAccessLayer.h"

class AsyncTaskRunner;

namespace PKG {
    class PackageManager : public QObject {
        Q_OBJECT
    private:
        explicit PackageManager(const QString &dbPath, QObject *parent = nullptr);

    protected:
        bool attachWorker(AsyncTaskRunner *worker);
//        QPointer<AsyncTaskRunner> initWorker();
        void removeWorker(const QUuid &id);

    public:
        static bool Init(const QString &dbPath, QObject *parent = nullptr);
        static void Destruct();
        static PackageManager *Instance();

    public slots:
        QUuid requestInstalledPackages();
        QUuid requestUserUpgradablePackages();
        QUuid requestSystemUpgradablePackages();
        QUuid requestCategoryPackages(const QString &categoryName);
        QUuid requestPackageInstallation(const QString &packageName);
        QUuid requestPackageUpdate(const QStringList &packageNameList);
        QUuid requestPackageRemoval(const QString &packageName);

    public:
        QVector<Category *> categoryList();

    signals:
        void installedPackagesReady(const QVector<Package *> &packageList);
        void userUpgradablePackagesReady(const QVector<Package *> &packageList);
        void systemUpgradablePackagesReady(const QVector<Package *> &packageList);
        void categoryPackagesReady(const QVector<Package *> &packageList);
        void packageInstalled(const QString &name);
        void packageUpdated(const QStringList &nameList);
        void packageRemoved(const QString &name);
        void newTaskData(const QUuid &taskId, const QString &data);
        void taskDone(const QUuid &taskId, const QString &data);
        void taskFailed(const QUuid &taskId, const QString &message);

    private:
        static PackageManager *_instance;
        DataAccessLayer *m_db;
        QMap<QUuid, QPointer<AsyncTaskRunner> > m_workerDict;
    };
}

#endif //PX_SOFTWARE_PACKAGEMANAGER_H
