//
// Created by Reza Alizadeh Majd on 10/21/19.
//

#include "PackageManager.h"
#include "GUIX/GuixInstalledPackagesTask.h"
#include "GUIX/GuixUpgradablePackagesTask.h"
#include "GUIX/GuixProfileStatusTask.h"
#include "GUIX/GuixPackageInstallTask.h"
#include "GUIX/GuixPackageUpgradeTask.h"
#include "GUIX/GuixPackageRemoveTask.h"
#include <QDebug>

namespace PKG {
    PackageManager *PackageManager::_instance = nullptr;

    PackageManager::PackageManager(const QString &dbPath, QObject *parent) : QObject(parent) {
        m_db = new DataAccessLayer(dbPath, this);
        m_wrapper = new GuixWrapper(this);
        m_wrapper->start();
        refreshProfile();
    }

    void PackageManager::refreshProfile(const std::function<void()> &callback, bool force) {
        m_profile.refreshed = false;
        QPointer<GuixProfileStatusTask> worker = new GuixProfileStatusTask(this);
        connect(worker, &GuixProfileStatusTask::profileIsReady, [=](const GuixProfile &profile) {
            m_profile = profile;
            if (callback != nullptr) {
                callback();
            }
        });
        connect(worker, &AsyncTaskRunner::failed, [=](const QString &message) {
            qDebug() << "Profile refresh failed:\n" << message;
        });
        m_wrapper->appendTask(worker, force);
    }

    bool PackageManager::prepareAndExec(GuixTask *worker, bool refresh) {
        if (worker != nullptr) {
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
            connect(worker, &AsyncTaskRunner::canceled, [=]() {
                emit taskCanceled(worker->Id());
            });
            bool succeed = m_wrapper->appendTask(worker);
            if (succeed && refresh) {
                refreshProfile();
            }
            return succeed;
        }
        return false;
    }

    QUuid PackageManager::getProfileAndPerform(const std::function<void(const QUuid &, const GuixProfile &)> &task) {
        QUuid workerId = QUuid::createUuid();
        auto *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, [=]() {
            if (m_profile.refreshed) {
                timer->stop();
                task(workerId, m_profile);
                timer->deleteLater();
            }
        });
        timer->start(100);
        return workerId;
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
        return getProfileAndPerform([=](const QUuid &taskId, const GuixProfile &profile) {
            auto dbPackages = m_db->packageList(profile.installedPackages);
            for (auto *pkg : dbPackages) {
                pkg->setInstalled(profile.installedPackages.contains(pkg->name()));
                pkg->setUpdateAvailable(profile.upgradablePackages.contains(pkg->name()));
            }
            emit installedPackagesReady(taskId, dbPackages);
        });
    }

    QUuid PackageManager::requestUserUpgradablePackages() {
        return getProfileAndPerform([=](const QUuid &worker_id, const GuixProfile &profile) {
            auto dbPackages = m_db->packageList(profile.upgradablePackages);
            for (auto *pkg : dbPackages) {
                pkg->setInstalled(profile.installedPackages.contains(pkg->name()));
                pkg->setUpdateAvailable(profile.upgradablePackages.contains(pkg->name()));
            }
            emit userUpgradablePackagesReady(worker_id, dbPackages);
        });
    }

    QUuid PackageManager::requestSystemUpgradablePackages() {
        // todo: need to find a way to get system-upgradable packages.
        QPointer<GuixUpgradablePackagesTask> worker = new GuixUpgradablePackagesTask(GuixPackageProfiles::SYSTEM, this);
        connect(worker, &GuixUpgradablePackagesTask::packageListReady, [=](const QStringList &guixPackages) {
            auto dbPackages = m_db->packageList(guixPackages);
            for (auto *pkg : dbPackages) {
                pkg->setInstalled(true);
                pkg->setUpdateAvailable(true);
            }
            emit systemUpgradablePackagesReady(worker->Id(), dbPackages);
        });
//        prepareAndExec(worker);
        return worker->Id();
    }

    QUuid PackageManager::requestCategoryPackages(const QString &categoryName) {
        return getProfileAndPerform([=](const QUuid &worker_id, const GuixProfile &profile) {
            auto dbPackages = m_db->categoryPackages(categoryName);
            for (auto *pkg : dbPackages) {
                pkg->setInstalled(profile.installedPackages.contains(pkg->name()));
                pkg->setUpdateAvailable(profile.upgradablePackages.contains(pkg->name()));
            }
            emit categoryPackagesReady(worker_id, dbPackages);
        });
    }

    QUuid PackageManager::requestPackageSearch(const QString &keyword) {
        return getProfileAndPerform([=](const QUuid &worker_id, const GuixProfile &profile) {
            auto dbPackages = m_db->findPackages(keyword);
            for (auto *pkg : dbPackages) {
                pkg->setInstalled(profile.installedPackages.contains(pkg->name()));
                pkg->setUpdateAvailable(profile.upgradablePackages.contains(pkg->name()));
            }
            emit packageSearchResultsReady(worker_id, dbPackages);
        });
    }

    QUuid PackageManager::requestTagPackages(const QString &tagName) {
        return getProfileAndPerform([=](const QUuid &worker_id, const GuixProfile &profile) {
            auto dbPackages = m_db->tagPackages(tagName);
            for (auto *pkg : dbPackages) {
                pkg->setInstalled(profile.installedPackages.contains(pkg->name()));
                pkg->setUpdateAvailable(profile.upgradablePackages.contains(pkg->name()));
            }
            emit tagPackagesReady(worker_id, dbPackages);
        });
    }

    QUuid PackageManager::requestPackageDetails(const QString &packageName) {
        return getProfileAndPerform([=](const QUuid &worker_id, const GuixProfile &profile) {
            auto pkg = m_db->packageDetails(packageName);
            if (pkg == nullptr) {
                emit taskFailed(worker_id, QString("package named: '%1' not found.").arg(packageName));
            } else {
                pkg->setInstalled(profile.installedPackages.contains(pkg->name()));
                pkg->setUpdateAvailable(profile.upgradablePackages.contains(pkg->name()));
                emit packageDetailsReady(worker_id, pkg);
            }
        });
    }

    QUuid PackageManager::requestPackageInstallation(const QString &packageName) {
        QPointer<GuixPackageInstallTask> worker = new GuixPackageInstallTask(packageName, this);
        connect(worker, &GuixPackageInstallTask::packageInstalled, [=](const QString &pkgName) {
            emit packageInstalled(worker->Id(), pkgName);
        });
        prepareAndExec(worker, true);
        return worker->Id();
    }

    QUuid PackageManager::requestPackageUpdate(const QStringList &packageNameList) {
        QPointer<GuixPackageUpdateTask> worker = new GuixPackageUpdateTask(packageNameList, this);
        connect(worker, &GuixPackageUpdateTask::packagesUpdated, [=](const QStringList &pkgList) {
            emit packageUpdated(worker->Id(), pkgList);
        });
        prepareAndExec(worker, true);
        return worker->Id();
    }

    QUuid PackageManager::requestPackageRemoval(const QString &packageName) {
        QPointer<GuixPackageRemovalTask> worker = new GuixPackageRemovalTask(packageName, this);
        connect(worker, &GuixPackageRemovalTask::packageRemoved, [=](const QString &pkgName) {
            emit packageRemoved(worker->Id(), packageName);
        });
        prepareAndExec(worker, true);
        return worker->Id();
    }

    bool PackageManager::requestTaskCancel(const QUuid &taskId) {
        return m_wrapper->cancelTask(taskId);
    }

    QVector<Category *> PackageManager::categoryList() {
        return m_db->categoryList();
    }
}
