//
// Created by Reza Alizadeh Majd on 10/21/19.
//

#include "PackageManager.h"

namespace PKG {
    PackageManager::PackageManager(const QString &dbPath, QObject *parent) : QObject(parent) {
        m_db = new DataAccessLayer(dbPath, this);
        m_parser = new GuixParser(m_db);
    }

    void PackageManager::requestInstalledPackages() {
        QPointer<AsyncTaskRunner> worker = new AsyncTaskRunner(this);
        connect(worker, &AsyncTaskRunner::done, [&](const QString &data) {
            auto packageList = m_parser->parseInstalledPackagesResponse(data);
            emit installedPackagesReady(packageList);
            qDeleteAll(packageList);
        });
        connect(worker, &AsyncTaskRunner::failed, this, &PackageManager::failed);
        worker->asyncRun("guix", QStringList() << "package"
                                               << "--list-installed");
    }

    void PackageManager::requestUserUpgradablePackages() {
        QPointer<AsyncTaskRunner> worker = new AsyncTaskRunner(this);
        connect(worker, &AsyncTaskRunner::done, [&](const QString &stdOut, const QString &stdErr) {
            auto pkgList = m_parser->parseUpdatePackageListResponse(stdErr);
            emit userUpgradablePackagesReady(pkgList);
            qDeleteAll(pkgList);
        });
        connect(worker, &AsyncTaskRunner::failed, this, &PackageManager::failed);
        worker->asyncRun("guix", QStringList() << "package"
                                               << "-n"
                                               << "-u");
    }

    void PackageManager::requestSystemUpgradablePackages() {
        QPointer<AsyncTaskRunner> worker = new AsyncTaskRunner(this);
        connect(worker, &AsyncTaskRunner::done, [&](const QString &stdOut, const QString &stdErr) {
            auto pkgList = m_parser->parseUpdatePackageListResponse(stdErr);
            emit systemUpgradablePackagesReady(pkgList);
            qDeleteAll(pkgList);
        });
        connect(worker, &AsyncTaskRunner::failed, this, &PackageManager::failed);
        worker->asyncRun("guix", QStringList() << "package"
                                               << "-p" << "/run/current-system/profile"
                                               << "-n"
                                               << "-u");
    }

    QUuid PackageManager::requestPackageInstallation(const QString &packageName) {
        QPointer<AsyncTaskRunner> worker = new AsyncTaskRunner(this);
        m_workerDict[worker->Id()] = worker;
        connect(worker, &AsyncTaskRunner::done, [=](const QString &stdOut, const QString &stdErr) {
            emit packageInstalled(packageName);
            m_workerDict.remove(worker->Id());
        });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            emit failed(message);
            m_workerDict.remove(worker->Id());
        });
        connect(worker, &AsyncTaskRunner::newData, [=](const QString &outData, const QString &errData) {
            if (!outData.isEmpty()) {
                emit newTaskData(worker->Id(), outData);
            } else if (!errData.isEmpty()) {
                emit newTaskData(worker->Id(), errData);
            }
        });
        worker->asyncRun("guix", QStringList() << "package" << "-i" << packageName);
        return worker->Id();
    }

    QUuid PackageManager::requestPackageUpdate(const QString &packageName) {
        QPointer<AsyncTaskRunner> worker = new AsyncTaskRunner(this);
        m_workerDict[worker->Id()] = worker;
        connect(worker, &AsyncTaskRunner::done, [=](const QString &outData, const QString &errData) {
            emit packageUpdated(packageName);
            emit taskDone(worker->Id());
            m_workerDict.remove(worker->Id());
        });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            emit failed(message);
            emit taskFailed(worker->Id(), message);
        });
        connect(worker, &AsyncTaskRunner::newData, [=](const QString &outData, const QString &errData) {
            if (!outData.isEmpty()) {
                emit newTaskData(worker->Id(), outData);
            } else if (!errData.isEmpty()) {
                emit newTaskData(worker->Id(), errData);
            }
        });
        worker->asyncRun("guix", QStringList() << "package" << "-u" << packageName);
        return worker->Id();
    }
}
