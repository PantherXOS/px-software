//
// Created by Reza Alizadeh Majd on 10/21/19.
//

#include "PackageManager.h"
#include "../AsyncTaskRunner.h"
#include "GUIX/GuixInstalledPackagesTask.h"
#include "GUIX/GuixUpgradablePackagesTask.h"
#include "GUIX/GuixProfileStatusTask.h"
#include <QDebug>

namespace PKG {
    PackageManager *PackageManager::_instance = nullptr;

    PackageManager::PackageManager(const QString &dbPath, QObject *parent) : QObject(parent) {
        m_db = new DataAccessLayer(dbPath, this);
    }

    bool PackageManager::attachWorker(AsyncTaskRunner *worker) {
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
        return true;
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
        QPointer<GuixInstalledPackagesTask> worker = new GuixInstalledPackagesTask(this);
        attachWorker(worker);
        auto worker_id = worker->Id();
        connect(worker, &GuixInstalledPackagesTask::packageListReady, [&](const QStringList &guixPackages) {
            auto packageList = m_db->packageList(guixPackages);
            for (auto *pkg : packageList) {
                pkg->setInstalled(true);
            }
            emit installedPackagesReady(worker_id, packageList);
            this->removeWorker(worker_id);
        });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            this->removeWorker(worker_id);
        });
        worker->asyncRun();
        return worker_id;
    }

    QUuid PackageManager::requestUserUpgradablePackages() {
        QPointer<GuixUpgradablePackagesTask> worker = new GuixUpgradablePackagesTask(GuixPackageProfiles::USER, this);
        attachWorker(worker);
        auto worker_id = worker->Id();
        connect(worker, &GuixUpgradablePackagesTask::packageListReady, [&](const QStringList &guixPackages) {
            auto packageList = m_db->packageList(guixPackages);
            for (auto *pkg : packageList) {
                pkg->setInstalled(true);
                pkg->setUpdateAvailable(true);
            }
            emit userUpgradablePackagesReady(worker_id, packageList);
            this->removeWorker(worker_id);
        });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            this->removeWorker(worker_id);
        });
        worker->asyncRun();
        return worker_id;
    }

    QUuid PackageManager::requestSystemUpgradablePackages() {
        QPointer<GuixUpgradablePackagesTask> worker = new GuixUpgradablePackagesTask(GuixPackageProfiles::SYSTEM, this);
        attachWorker(worker);
        auto worker_id = worker->Id();
        connect(worker, &GuixUpgradablePackagesTask::packageListReady, [=](const QStringList &guixPackages) {
            auto packageList = m_db->packageList(guixPackages);
            for (auto *pkg : packageList) {
                pkg->setInstalled(true);
                pkg->setUpdateAvailable(true);
            }
            emit systemUpgradablePackagesReady(worker_id, packageList);
            this->removeWorker(worker_id);
        });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            this->removeWorker(worker_id);
        });
        worker->asyncRun();
        return worker_id;
    }

    QUuid PackageManager::requestCategoryPackages(const QString &categoryName) {
        QPointer<GuixProfileStatusTask> worker = new GuixProfileStatusTask(this);
        attachWorker(worker);
        auto worker_id = worker->Id();
        connect(worker, &GuixProfileStatusTask::packageListReady,
                [=](const QStringList &installedPackages, const QStringList &upgradablePackage) {
                    auto dbPackages = m_db->categoryPackages(categoryName);
                    for (auto *pkg : dbPackages) {
                        pkg->setInstalled(installedPackages.contains(pkg->name()));
                        pkg->setUpdateAvailable(upgradablePackage.contains(pkg->name()));
                    }
                    emit categoryPackagesReady(worker_id, dbPackages);
                    this->removeWorker(worker_id);
                });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            this->removeWorker(worker_id);
        });
        worker->asyncRun();
        return worker_id;
    }

    QUuid PackageManager::requestPackageSearch(const QString &keyword) {
        QPointer<GuixProfileStatusTask> worker = new GuixProfileStatusTask(this);
        attachWorker(worker);
        auto worker_id = worker->Id();
        connect(worker, &GuixProfileStatusTask::packageListReady,
                [=](const QStringList &installedPackages, const QStringList &upgradablePackages) {
                    auto dbPackages = m_db->findPackages(keyword);
                    for (auto *pkg : dbPackages) {
                        pkg->setInstalled(installedPackages.contains(pkg->name()));
                        pkg->setUpdateAvailable(upgradablePackages.contains(pkg->name()));
                    }
                    emit packageSearchResultsReady(worker_id, dbPackages);
                    this->removeWorker(worker_id);
                });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            this->removeWorker(worker_id);
        });
        worker->asyncRun();
        return worker_id;
    }

    QUuid PackageManager::requestPackageDetails(const QString &packageName) {
        QPointer<GuixProfileStatusTask> worker = new GuixProfileStatusTask(this);
        attachWorker(worker);
        auto worker_id = worker->Id();
        connect(worker, &GuixProfileStatusTask::packageListReady,
                [=](const QStringList &installedPackages, const QStringList &upgradablePackages) {
                    auto pkg = m_db->packageDetails(packageName);
                    if (pkg == nullptr) {
                        emit taskFailed(worker_id, QString("package named: '%1' not found.")
                                .arg(packageName));
                    } else {
                        pkg->setInstalled(installedPackages.contains(pkg->name()));
                        pkg->setUpdateAvailable(upgradablePackages.contains(pkg->name()));
                        emit packageDetailsReady(worker_id, pkg);
                    }
                    this->removeWorker(worker_id);
                });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            this->removeWorker(worker_id);
        });
        worker->asyncRun();
        return worker_id;
    }

    QUuid PackageManager::requestPackageInstallation(const QString &packageName) {
        QPointer<AsyncTaskRunner> worker = new AsyncTaskRunner(this);
        attachWorker(worker);
        auto worker_id = worker->Id();
        connect(worker, &AsyncTaskRunner::done, [=](const QString &stdOut, const QString &stdErr) {
            emit packageInstalled(worker_id, packageName);
            this->removeWorker(worker_id);
        });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            this->removeWorker(worker_id);
        });
        worker->asyncRun("guix", QStringList() << "package" << "-i" << packageName);
        return worker_id;
    }

    QUuid PackageManager::requestPackageUpdate(const QStringList &packageNameList) {
        QPointer<AsyncTaskRunner> worker = new AsyncTaskRunner(this);
        attachWorker(worker);
        auto worker_id = worker->Id();
        connect(worker, &AsyncTaskRunner::done, [=](const QString &outData, const QString &errData) {
            emit packageUpdated(worker_id, packageNameList);
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
        QPointer<AsyncTaskRunner> worker = new AsyncTaskRunner(this);
        attachWorker(worker);
        auto worker_id = worker->Id();
        connect(worker, &AsyncTaskRunner::done, [=](const QString &outData, const QString &errData) {
            emit packageRemoved(worker_id, packageName);
            this->removeWorker(worker_id);
        });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            this->removeWorker(worker_id);
        });
        worker->asyncRun("guix", QStringList() << "package" << "-r" << packageName);
        return worker_id;
    }

    bool PackageManager::requestTaskCancel(const QUuid &taskId) {
        if (m_workerDict.contains(taskId)) {
            m_workerDict[taskId]->close();
            emit taskCanceled(taskId);
            return true;
        }
        return false;
    }

    QVector<Category *> PackageManager::categoryList() {
        return m_db->categoryList();
    }
}
