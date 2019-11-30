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
        QUuid requestPackageSearch(const QString &keyword);
        QUuid requestTagPackages(const QString &tagName);
        QUuid requestPackageDetails(const QString &packageName);
        QUuid requestPackageInstallation(const QString &packageName);
        QUuid requestPackageUpdate(const QStringList &packageNameList);
        QUuid requestPackageRemoval(const QString &packageName);

        bool requestTaskCancel(const QUuid &taskId);

    public:
        QVector<Category *> categoryList();

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
        QMap<QUuid, QPointer<AsyncTaskRunner> > m_workerDict;
    };
}

#endif //PX_SOFTWARE_PACKAGEMANAGER_H
