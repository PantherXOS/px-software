//
// Created by Reza Alizadeh Majd on 10/21/19.
//

#include "PackageManager.h"
#include <QDebug>

namespace PKG {
    PackageManager *PackageManager::_instance = nullptr;

    PackageManager::PackageManager(const QString &dbPath, QObject *parent) : QObject(parent) {
        m_db = new DataAccessLayer(dbPath, this);
        m_parser = new GuixParser(m_db);
    }

    QPointer<AsyncTaskRunner> PackageManager::initWorker() {
        QPointer<AsyncTaskRunner> worker = new AsyncTaskRunner(this);
        m_workerDict[worker->Id()] = worker;
        connect(worker, &AsyncTaskRunner::done, [=](const QString &outData, const QString &errData) {
            QString data = outData + errData;
            emit taskDone(worker->Id(), data);
        });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            emit taskFailed(worker->Id(), message);
        });
        connect(worker, &AsyncTaskRunner::newData, [=](const QString &outData, const QString &errData) {
            if (!outData.isEmpty()) {
                emit newTaskData(worker->Id(), outData);
            } else if (!errData.isEmpty()) {
                emit newTaskData(worker->Id(), errData);
            }
        });
        return worker;
    }

    void PackageManager::removeWorker(const QUuid &id) {
        if (m_workerDict.contains(id)) {
            auto worker = m_workerDict[id];
            m_workerDict.remove(id);
            worker->deleteLater();
        }
    }

    bool PackageManager::Init(const QString &dbPath, QObject *parent) {
        if (PackageManager::_instance != nullptr) {
            qWarning() << "already inited";
            return false;
        } else {
            PackageManager::_instance = new PackageManager(dbPath, parent);
            return true;
        }
    }

    void PackageManager::Destruct() {
        if (PackageManager::_instance != nullptr) {
            PackageManager::_instance->deleteLater();
            PackageManager::_instance = nullptr;
        }
    }

    PackageManager *PackageManager::Instance() {
        if (PackageManager::_instance == nullptr) {
            qCritical() << "PackageManager is not initiated";
        }
        return PackageManager::_instance;
    }

    QUuid PackageManager::requestInstalledPackages() {
        auto worker = this->initWorker();
        auto worker_id = worker->Id();
        connect(worker, &AsyncTaskRunner::done, [&](const QString &data) {
            auto packageList = m_parser->parseInstalledPackagesResponse(data);
            emit installedPackagesReady(packageList);
            this->removeWorker(worker_id);
        });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            this->removeWorker(worker_id);
        });
        worker->asyncRun("guix", QStringList() << "package" << "--list-installed");
        return worker_id;
    }

    QUuid PackageManager::requestUserUpgradablePackages() {
        auto worker = this->initWorker();
        auto worker_id = worker->Id();
        connect(worker, &AsyncTaskRunner::done, [=](const QString &stdOut, const QString &stdErr) {
            auto pkgList = m_parser->parseUpdatePackageListResponse(stdErr);
            emit userUpgradablePackagesReady(pkgList);
            this->removeWorker(worker_id);
        });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            this->removeWorker(worker_id);
        });
        worker->asyncRun("guix", QStringList() << "package"
                                               << "-n"
                                               << "-u");
        return worker_id;
    }

    QUuid PackageManager::requestSystemUpgradablePackages() {
        auto worker = this->initWorker();
        auto worker_id = worker->Id();
        connect(worker, &AsyncTaskRunner::done, [=](const QString &stdOut, const QString &stdErr) {
            auto pkgList = m_parser->parseUpdatePackageListResponse(stdErr);
            emit systemUpgradablePackagesReady(pkgList);
            this->removeWorker(worker_id);
        });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            this->removeWorker(worker_id);
        });
        worker->asyncRun("guix", QStringList() << "package"
                                               << "-p" << "/run/current-system/profile"
                                               << "-n"
                                               << "-u");
        return worker_id;
    }

    QUuid PackageManager::requestPackageInstallation(const QString &packageName) {
        auto worker = this->initWorker();
        auto worker_id = worker->Id();
        connect(worker, &AsyncTaskRunner::done, [=](const QString &stdOut, const QString &stdErr) {
            emit packageInstalled(packageName);
            this->removeWorker(worker_id);
        });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            this->removeWorker(worker_id);
        });
        worker->asyncRun("guix", QStringList() << "package" << "-i" << packageName);
        return worker_id;
    }

    QUuid PackageManager::requestPackageUpdate(const QStringList &packageNameList) {
        auto worker = this->initWorker();
        auto worker_id = worker->Id();
        connect(worker, &AsyncTaskRunner::done, [=](const QString &outData, const QString &errData) {
            emit packageUpdated(packageNameList);
            this->removeWorker(worker_id);
        });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            this->removeWorker(worker_id);
        });
        QStringList args;
        args << "package" << "-u";
        for (const auto &pkgName : packageNameList) {
            args << pkgName;
        }
        worker->asyncRun("guix", args);
        return worker_id;
    }

    QUuid PackageManager::requestPackageRemoval(const QString &packageName) {
        auto worker = this->initWorker();
        auto worker_id = worker->Id();
        connect(worker, &AsyncTaskRunner::done, [=](const QString &outData, const QString &errData) {
            emit packageRemoved(packageName);
            this->removeWorker(worker_id);
        });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            this->removeWorker(worker_id);
        });
        worker->asyncRun("guix", QStringList() << "package" << "-r" << packageName);
        return worker_id;
    }
}
