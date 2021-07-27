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
#include "GUIX/PxUpdateTask.h"
#include <QDebug>
#include <QStandardPaths>
#include <FileDownloader.h>
#include <QEventLoop>
#include <yaml-cpp/yaml.h>
#include <GUIX/GuixPackageSearchTask.h>

#define SOFTWARE_ASSET_META_LOCAL_FILE      QString("px-software-assets_latest_meta.yaml")

#define SOFTWARE_ASSET_BASE_URL             QString("https://assets.software.pantherx.org")
#define SOFTWARE_ASSET_ARCHIVE_FILE_URL     SOFTWARE_ASSET_BASE_URL + QString("/") + SOFTWARE_LATEST_ARCHIVE_FILE_NAME

#define SOFTWARE_ASSET_LATEST_META_LOCAL_PATH       QStandardPaths::standardLocations(QStandardPaths::GenericCacheLocation)[0] + QString("/px-software-assets/")
#define SOFTWARE_ASSET_LATEST_META_LOCAL_FILE       SOFTWARE_ASSET_LATEST_META_LOCAL_PATH + SOFTWARE_ASSET_META_LOCAL_FILE

namespace PKG {
    PackageManager *PackageManager::_instance = nullptr;

    PackageManager::PackageManager(const QString &dbPath, QObject *parent) : QObject(parent) {
        m_dbPath = dbPath;
        reload();
    }

    void PackageManager::reload(){
        qDebug() << "Database loaded from: " << m_dbPath;
        m_db = new DataAccessLayer(m_dbPath, this);
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

    void PackageManager::updateDB(const QString &installedMetaFile, const QString dbUrl){
        if(installedMetaFile.isEmpty() || dbUrl.isEmpty()) {
            qWarning() << "Failure in downloading the DB file (installedMetaFile:" + installedMetaFile + " ,Url:" + dbUrl +")!";  
            emit dbUpdated(false);
            return;
        }

        FileDownloader *downloader = new FileDownloader(this);
        connect(downloader, &FileDownloader::downloaded, [=](const QString &path){
            qDebug() << "New Asset archive file downloaded from " + dbUrl + " in " + path;
            QFile tarFile(path);
            bool result = false;
            if(tarFile.exists() && tarFile.size() > 0){
                QDir dbPath(m_dbPath);
                dbPath.removeRecursively();
                QDir().mkpath(m_dbPath);

                std::string extractCommand = "tar --strip-components=1 -xvf " + path.toStdString() +
                                            " -C " + m_dbPath.toStdString();
                qDebug() << "Running:" << QString::fromStdString(extractCommand);
                if(!system(extractCommand.c_str())){
                    result = true;
                    qDebug() << "New DB assets extracted to:" << m_dbPath;
                    QFile localMetaFile(SOFTWARE_ASSET_LATEST_META_LOCAL_FILE);
                    localMetaFile.moveToTrash();
                    bool cpResult = QFile::copy(installedMetaFile, SOFTWARE_ASSET_LATEST_META_LOCAL_FILE);
                    if(!cpResult)
                        qWarning() << "Warning in file copy from" << installedMetaFile << "to" << SOFTWARE_ASSET_LATEST_META_LOCAL_FILE;
                } else {
                    qWarning() << "Error occured in extracting the DB: " + QString::fromStdString(extractCommand);
                }
            } else {
                qDebug() << "File not exist or not a tar file (" << path << ")";
            }
            tarFile.moveToTrash();
            emit dbUpdated(result);
            // downloader->deleteLater();
        });
        downloader->start(QUrl(dbUrl),"/tmp/");
    }

    QUrl PackageManager::getInstalledMetaFile(){
        auto list = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
        for(auto const &l: list){
            qDebug() << "Finding wiki in: \"" + l + "\" ...";
            QString metaPath = l + "/px-software-assets-meta"; 
            QFileInfo metaFile(metaPath + "/" + SOFTWARE_ASSET_META_LOCAL_FILE);
            if(metaFile.exists()){
                qDebug() << "PantherX Software meta file found at: \"" + metaPath + "\"" ;
                return metaFile.filePath();
            }
        }
        return QUrl("");
    }

    QMap<QString, QString> PackageManager::getMetaInfo(const QString &metaFile){
        QMap<QString, QString> map;
        YAML::Node currentMetaYaml = YAML::LoadFile(metaFile.toStdString());
        if (currentMetaYaml["hash"] && currentMetaYaml["datestamp"]) {
            auto hash = currentMetaYaml["hash"].as<std::string>();
            map["hash"] = QString::fromStdString(hash);
            auto date = currentMetaYaml["datestamp"].as<std::string>();
            map["date"] = QString::fromStdString(date);
            map["db_url"] = SOFTWARE_ASSET_BASE_URL + "/" + QString::fromStdString(date) + "_" + QString::fromStdString(hash) + ".tgz";
        }
        return map;
    }
    
    void PackageManager::checkDBupdate(){
        qDebug() << "Check Software db updates ...";
        // install `px-software-assets-meta`
        connect(&updaterTaskRunner, &AsyncTaskRunner::failed, [&](const QString &message){
            // use the latest db update
            qWarning() << "Installing Software Assets Meta File: Failed";
            qWarning() << message;
            emit dbUpdated(false);
        });
        connect(&updaterTaskRunner, &AsyncTaskRunner::done, [&](const QString &message){
            QFile localAssetFile(SOFTWARE_ASSET_LATEST_META_LOCAL_FILE);
            auto installedMetaFile = getInstalledMetaFile();
            QMap<QString, QString> map;
            if(QFile(installedMetaFile.toString()).exists()) {
                map = getMetaInfo(installedMetaFile.toString());
            } else {
                qWarning() << "installed meta file does not exist! (" + installedMetaFile.toString() + ")";
                emit dbUpdated(false);
            }

            if(!localAssetFile.exists()) {
                // if local meta file doesn't exists
                //   copy meta to cache -> download db -> extract -> return
                QDir().mkpath(SOFTWARE_ASSET_LATEST_META_LOCAL_PATH);
                qDebug() << SOFTWARE_ASSET_LATEST_META_LOCAL_PATH + " created!";
                updateDB(installedMetaFile.toString(), map["db_url"]);
                return;
            }
            // local assets are availe: yes
            if(QFile(installedMetaFile.toString()).exists()) {
                auto installedMetaYamlMap = getMetaInfo(installedMetaFile.toString());
                auto localMetaYamlMap = getMetaInfo(SOFTWARE_ASSET_LATEST_META_LOCAL_FILE);
                // Compare
                if (!installedMetaYamlMap["hash"].isEmpty() && !localMetaYamlMap["hash"].isEmpty() && 
                    !installedMetaYamlMap["date"].isEmpty() && !localMetaYamlMap["date"].isEmpty()) {
                    auto installedHash = installedMetaYamlMap["hash"];
                    auto localHash = localMetaYamlMap["hash"];
                    if(localHash == installedHash) {
                        qDebug() << "Software DB is up-to-date (hash: " + installedHash + ")";
                        emit dbUpdated(false);
                        return;
                    }

                    auto installedDate = installedMetaYamlMap["date"];
                    auto localDate = localMetaYamlMap["date"];
                    QDateTime installedDateObj = QDateTime::fromString(installedDate,"dd-MM-yyyy_hh-mm-ss");
                    installedDateObj.setTimeSpec(Qt::UTC);
                    QDateTime localDateObj = QDateTime::fromString(localDate,"dd-MM-yyyy_hh-mm-ss");
                    localDateObj.setTimeSpec(Qt::UTC);
                    if(localDateObj >= installedDateObj){
                        emit dbUpdated(false);
                        return;
                    }
                    qDebug() << "Software DB Update is available. (current hash: " + localHash + ", latest hash: " + installedHash + ")";
                }
                updateDB(installedMetaFile.toString(), map["db_url"]);
            } else {
                qWarning() << "Installed Meta file not found: " << installedMetaFile.toString();
                emit dbUpdated(false);
                return;
            }
            

            //   compare local meta with installed meta : installed is older -> return
            //                                          : installed is same  -> return
            //                                          : installed is newer -> download db -> extract -> return
        
        });
        updaterTaskRunner.asyncRun("guix", QStringList{"package", "-i" ,"px-software-assets-meta"});
        
        
        
        // QDir localAssetPath(SOFTWARE_ASSET_LATEST_META_LOCAL_PATH);
        // if(!localAssetPath.exists()) {
        //     QDir().mkpath(SOFTWARE_ASSET_LATEST_META_LOCAL_PATH);
        //     qDebug() << SOFTWARE_ASSET_LATEST_META_LOCAL_PATH + " created!";
        // }            

        // // Download Meta File
        // FileDownloader *downloader = new FileDownloader(this);
        // connect(downloader, &FileDownloader::downloaded, [&](const QString &path){
        //     qDebug() << "Meta file downloaded from " + SOFTWARE_ASSET_META_FILE_URL + " in " + path + "";
        //     // downloader->deleteLater();
        //     // Check Meta file is exist locally
        //     QFile currentMetaFile(SOFTWARE_ASSET_CURRENT_META_LOCAL_FILE);
        //     QFile latestMetaFile(SOFTWARE_ASSET_LATEST_META_LOCAL_FILE);
        //     if(!currentMetaFile.exists()){
        //         qDebug() << "Meta file is not exist in " + SOFTWARE_ASSET_CURRENT_META_LOCAL_FILE;
        //         latestMetaFile.rename(SOFTWARE_ASSET_CURRENT_META_LOCAL_FILE);
        //         // Download new archive file
        //         updateDB();
        //         return;
        //     }

        //     YAML::Node currentMetaYaml = YAML::LoadFile((SOFTWARE_ASSET_CURRENT_META_LOCAL_FILE).toStdString());
        //     YAML::Node latestMetaYaml = YAML::LoadFile((SOFTWARE_ASSET_LATEST_META_LOCAL_FILE).toStdString());
        //     currentMetaFile.moveToTrash();
        //     latestMetaFile.rename(SOFTWARE_ASSET_CURRENT_META_LOCAL_FILE);
            
        //     // DB path is exists?
        //     QDir dbPath(m_dbPath);
        //     if(!dbPath.exists()){
        //         updateDB();
        //         return;
        //     }

        //     // Compare
        //     if (latestMetaYaml["hash"] && currentMetaYaml["hash"]) {
        //         auto latestHash = latestMetaYaml["hash"].as<std::string>();
        //         auto currentHash = currentMetaYaml["hash"].as<std::string>();
        //         if(latestHash == currentHash) {
        //             qDebug() << "Software DB is up-to-date (hash: " + QString::fromStdString(currentHash) + ")";
        //             emit dbUpdated(true);
        //             return;
        //         }
        //         qDebug() << "Software DB Update is available. (current hash: " + QString::fromStdString(currentHash) + ", latest hash: " + QString::fromStdString(latestHash) + ")";
        //     }
        //     updateDB();
        // });
        // downloader->start(QUrl(SOFTWARE_ASSET_META_FILE_URL),SOFTWARE_ASSET_LATEST_META_LOCAL_PATH);
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

    QUuid PackageManager::requestSystemUpdate() {
        QPointer<PxUpdateTask> worker;
        if(!systemIsInUpdating){
            worker = new PxUpdateTask(this);
            connect(worker, &PxUpdateTask::systemUpdateFinished, [&](const QString &outData, const QString &errData) {
                emit systemUpdateFinished(outData, errData);
                systemIsInUpdating = false;
            });
            prepareAndExec(worker);
        }
        return worker->Id();
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
        QPointer<GuixPackageSearchTask> worker = new GuixPackageSearchTask(keyword, this);
        connect(worker, &GuixPackageSearchTask::searchResultReady, [=](const QVector<Package *> packageList) {
            QVector<Package *> _packageList;
            auto dbPackages = m_db->findPackages(keyword);
            for (auto *_pkg : packageList) {
                Package *packageToAppend = nullptr;
                for (auto *pkg : dbPackages) {
                    if((pkg->name() == _pkg->name()) && (pkg->version() == _pkg->version())){
                        qDebug() << pkg->name() + "@" + pkg->version() << "is available in software db";
                        pkg->setAvailableInDB(true);
                        packageToAppend = pkg;
                        break;
                    }
                }
                if(!packageToAppend){
                    qDebug() << _pkg->name() + "@" + _pkg->version()  << "found but not available in software db";
                    _pkg->setAvailableInDB(false);
                    packageToAppend = _pkg;
                } else _pkg->deleteLater();
                packageToAppend->setInstalled(m_profile.installedPackages.contains(packageToAppend->name()));
                packageToAppend->setUpdateAvailable(m_profile.upgradablePackages.contains(packageToAppend->name()));
                _packageList.push_back(packageToAppend);
            }
            emit packageSearchResultsReady(worker->Id(), _packageList);
        });
        prepareAndExec(worker, true);
        return worker->Id();
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
