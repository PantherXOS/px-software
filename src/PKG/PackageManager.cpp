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

#include "PackageManager.h"
#include "GUIX/GuixInstalledPackagesTask.h"
#include "GUIX/GuixUpgradablePackagesTask.h"
#include "GUIX/GuixProfileStatusTask.h"
#include "GUIX/GuixPackageInstallTask.h"
#include "GUIX/GuixPackageUpgradeTask.h"
#include "GUIX/GuixPackageRemoveTask.h"
#include "GUIX/GuixProfileUpdateThread.h"
#include <QDebug>
#include <QStandardPaths>
#include <FileDownloader.h>
#include <QEventLoop>
#include <yaml-cpp/yaml.h>

#define SOFTWARE_LATEST_META_FILE_NAME      QString("px-software-assets_latest_meta.yaml")
#define SOFTWARE_CURRENT_META_FILE_NAME     QString("px-software-assets_current_meta.yaml")
#define SOFTWARE_LATEST_ARCHIVE_FILE_NAME   QString("px-software-assets_latest.tgz")

#define SOFTWARE_ASSET_BASE_URL             QString("https://assets.software.pantherx.org")
#define SOFTWARE_ASSET_ARCHIVE_FILE_URL     SOFTWARE_ASSET_BASE_URL + QString("/") + SOFTWARE_LATEST_ARCHIVE_FILE_NAME
#define SOFTWARE_ASSET_META_FILE_URL        SOFTWARE_ASSET_BASE_URL + QString("/") + SOFTWARE_LATEST_META_FILE_NAME

#define SOFTWARE_ASSETS_DOWNLOAD_LOCAL_PATH         QStandardPaths::standardLocations(QStandardPaths::GenericCacheLocation)[0] + QString("/px-software-assets/")
#define SOFTWARE_ASSETS_DB_LOCAL_PATH               SOFTWARE_ASSETS_DOWNLOAD_LOCAL_PATH + QString("db/")
#define SOFTWARE_ASSET_LATEST_META_LOCAL_FILE       SOFTWARE_ASSETS_DOWNLOAD_LOCAL_PATH + SOFTWARE_LATEST_META_FILE_NAME
#define SOFTWARE_ASSET_CURRENT_META_LOCAL_FILE      SOFTWARE_ASSETS_DOWNLOAD_LOCAL_PATH + SOFTWARE_CURRENT_META_FILE_NAME

namespace PKG {
    PackageManager *PackageManager::_instance = nullptr;

    PackageManager::PackageManager(QString dbPath, QObject *parent) : QObject(parent) {
        if(dbPath.isEmpty()) {
            checkDBupdate();
            dbPath = SOFTWARE_ASSETS_DB_LOCAL_PATH;
        }
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

    bool PackageManager::prepareAndExec(AsyncTaskRunner *worker, bool refresh) {
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

    void PackageManager::updateDB(){
        dbUpdating = true;
        QEventLoop loop;
        FileDownloader *downloader = new FileDownloader(this);
        connect(downloader, &FileDownloader::downloaded, [&](const QString &path){
            qDebug() << "New Asset archive file downloaded from " + SOFTWARE_ASSET_ARCHIVE_FILE_URL + " in " + path;

            QDir dbPath(SOFTWARE_ASSETS_DB_LOCAL_PATH);
            dbPath.removeRecursively();
            QDir().mkpath(SOFTWARE_ASSETS_DB_LOCAL_PATH);

            std::string extractCommand = "tar --strip-components=1 -xvf " + path.toStdString() +
                                         " -C " + (SOFTWARE_ASSETS_DB_LOCAL_PATH).toStdString();
            qDebug() << "Running:" << QString::fromStdString(extractCommand);
            system(extractCommand.c_str());
            QFile tarFile(path);
            tarFile.moveToTrash();
            loop.quit();
        });
        downloader->start(QUrl(SOFTWARE_ASSET_ARCHIVE_FILE_URL),"/tmp/");
        loop.exec();
        downloader->deleteLater();
        qDebug() << "New DB assets extracted to:" << SOFTWARE_ASSETS_DB_LOCAL_PATH;
    }

    void PackageManager::checkDBupdate(){
        QEventLoop loop;
        // 
        QDir localAssetPath(SOFTWARE_ASSETS_DOWNLOAD_LOCAL_PATH);
        if(!localAssetPath.exists()) {
            QDir().mkpath(SOFTWARE_ASSETS_DOWNLOAD_LOCAL_PATH);
            qDebug() << SOFTWARE_ASSETS_DOWNLOAD_LOCAL_PATH + " created!";
        }            

        // Download Meta File
        FileDownloader *downloader = new FileDownloader(this);
        connect(downloader, &FileDownloader::downloaded, [&](const QString &path){
            qDebug() << "Meta file downloaded from " + SOFTWARE_ASSET_META_FILE_URL + " in " + path + "";
            loop.quit();
        });
        downloader->start(QUrl(SOFTWARE_ASSET_META_FILE_URL),SOFTWARE_ASSETS_DOWNLOAD_LOCAL_PATH);
        loop.exec();
        downloader->deleteLater();

        // Check Meta file is exist locally
        QFile currentMetaFile(SOFTWARE_ASSET_CURRENT_META_LOCAL_FILE);
        QFile latestMetaFile(SOFTWARE_ASSET_LATEST_META_LOCAL_FILE);
        if(!currentMetaFile.exists()){
            qDebug() << "Meta file is not exist in " + SOFTWARE_ASSET_CURRENT_META_LOCAL_FILE;
            latestMetaFile.rename(SOFTWARE_ASSET_CURRENT_META_LOCAL_FILE);
            // Download new archive file
            updateDB();
            return;
        } 

        YAML::Node currentMetaYaml = YAML::LoadFile((SOFTWARE_ASSET_CURRENT_META_LOCAL_FILE).toStdString());
        YAML::Node latestMetaYaml = YAML::LoadFile((SOFTWARE_ASSET_LATEST_META_LOCAL_FILE).toStdString());
        latestMetaFile.moveToTrash();
        
        // DB path is exists?
        QDir dbPath(SOFTWARE_ASSETS_DB_LOCAL_PATH);
        if(!dbPath.exists()){
            updateDB();
            return;
        }

        // Compare
        if (latestMetaYaml["hash"] && currentMetaYaml["hash"]) {
            auto latestHash = latestMetaYaml["hash"].as<std::string>();
            auto currentHash = currentMetaYaml["hash"].as<std::string>();
            if(latestHash == currentHash) {
                qDebug() << "Software DB is up-to-date (hash: " + QString::fromStdString(currentHash) + ")";
                return;
            }
            qDebug() << "Software DB Update is available. (current hash: " + QString::fromStdString(currentHash) + ", latest hash: " + QString::fromStdString(latestHash) + ")";
        }
        updateDB();
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

    bool PackageManager::isUpdating(){
        return dbUpdating;
    }

    bool PackageManager::isInited(){
        return m_db->isInitted();
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
        prepareAndExec(worker);
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

    void PackageManager::requestDBPackageUpdate(){
        auto profileUpdateThread = new GuixProfileUpdateThread();
        connect(profileUpdateThread, &GuixProfileUpdateThread::error, [=](const QString &result) {
            emit dbUpdateError(result);
        });
        profileUpdateThread->start();
    }

    bool PackageManager::requestTaskCancel(const QUuid &taskId) {
        return m_wrapper->cancelTask(taskId);
    }

    QVector<Category *> PackageManager::categoryList() {
        return m_db->categoryList();
    }
}
