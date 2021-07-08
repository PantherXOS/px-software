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

#include <QStandardPaths>
#include "PackageManagerTracker.h"

#define SOFTWARE_ASSETS_DOWNLOAD_LOCAL_PATH         QStandardPaths::standardLocations(QStandardPaths::GenericCacheLocation)[0] + QString("/px-software-assets/")
#define SOFTWARE_ASSETS_DB_LOCAL_PATH               SOFTWARE_ASSETS_DOWNLOAD_LOCAL_PATH + QString("db")

PackageManagerTracker *PackageManagerTracker::_instance = nullptr;
QString PackageManagerTracker::_dbPath;

void PackageManagerTracker::init(const QString &dbPath) {
    if(_instance==nullptr){
        if(dbPath.isEmpty())
            _dbPath = SOFTWARE_ASSETS_DB_LOCAL_PATH;
        else 
            _dbPath = dbPath;
        PKG::PackageManager::Init(_dbPath, nullptr);
        _instance = new PackageManagerTracker();
    }
}

PackageManagerTracker *PackageManagerTracker::Instance() {
    if(_instance==nullptr){
        qCritical() << "PackageManagerTracker is not initiated";
    }
    return _instance;
}

bool PackageManagerTracker::packageInProgress(const QString &packageName){
    for(const auto &p : inProgressPackagesMap)
        if(p.second.name==packageName)
            return true;
    return false;
}

bool PackageManagerTracker::packageInProgress(const QString &packageName, QUuid &taskId) {
    for(const auto &p : inProgressPackagesMap)
        if(p.second.name == packageName) {
            taskId = p.first;
            return true;
        }
    return false;
}

bool PackageManagerTracker::packageInProgress(const QUuid &taskId) {
    if(inProgressPackagesMap.count(taskId)>0)
        return true;
    return false;
}

PackageManagerTracker::PackageManagerTracker(){
    m_pkgMgr = PKG::PackageManager::Instance();
    connect(m_pkgMgr, SIGNAL(installedPackagesReady(
                                     const QUuid &,
                                     const QVector<Package *>)), this, SLOT(installedPackageListHandler(
                                                                                    const QUuid &, const QVector<Package *>)));
    connect(m_pkgMgr, SIGNAL(userUpgradablePackagesReady(
                                     const QUuid &,
                                     const QVector<Package *>)), this, SLOT(userUpdatablePackageListHandler(
                                                                                    const QUuid &, const QVector<Package *>)));
    connect(m_pkgMgr, SIGNAL(systemUpgradablePackagesReady(
                                     const QUuid &,
                                     const QVector<Package *>)), this, SLOT(systemUpdatablePackageListHandler(
                                                                                    const QUuid &, const QVector<Package *>)));
    connect(m_pkgMgr, SIGNAL(packageInstalled(const QUuid &,const QString &)),this, SLOT(packageInstalledHandler(const QUuid &,const QString &)));
    connect(m_pkgMgr, SIGNAL(packageUpdated(const QUuid &,const QStringList &)),this, SLOT(packageUpdatedHandler(const QUuid &,const QStringList &)));
    connect(m_pkgMgr, SIGNAL(packageRemoved(const QUuid &,const QString &)),this, SLOT(packageRemovedHandler(const QUuid &,const QString &)));
    connect(m_pkgMgr, SIGNAL(taskFailed(const QUuid &, const QString &)),this, SLOT(taskFailedHandler(const QUuid &, const QString &)));
    connect(m_pkgMgr, SIGNAL(newTaskData(const QUuid &, const QString &)), this, SLOT(taskDataHandler(const QUuid &, const QString &)));
    connect(m_pkgMgr, SIGNAL(taskDone(const QUuid &, const QString &)), this, SLOT(taskDoneHandler(const QUuid &, const QString &)));
    connect(m_pkgMgr, SIGNAL(taskCanceled(const QUuid &)), this, SLOT(packageTaskCanceledHandler(const QUuid &)));
    connect(m_pkgMgr, &PKG::PackageManager::systemUpdateFinished, [=](const QString &outData, const QString &errData) {
        emit systemUpdateFinished(outData, errData);
        requestSystemUpdatablePackageList();
    });
}

QVector<Category *> PackageManagerTracker::categoryList() {
    return m_pkgMgr->categoryList();
}

bool PackageManagerTracker::requestPackageInstallation(const QString &packageName) {
    if(!packageInProgress(packageName)) {
        InProgressPackage inProgressPackage;
        inProgressPackage.name = packageName;
        inProgressPackage.status = PackageStatus::INSTALLING;
        inProgressPackagesMap[m_pkgMgr->requestPackageInstallation(packageName)] = inProgressPackage;
        emit inProgressRequest();
        return true;
    }
    return false;
}

bool PackageManagerTracker::requestPackageUpdate(const QString &packageName) {
    if(!packageInProgress(packageName)) {
        InProgressPackage inProgressPackage;
        inProgressPackage.name = packageName;
        inProgressPackage.status = PackageStatus::UPDATING;
        QStringList packageNames = {packageName};
        inProgressPackagesMap[m_pkgMgr->requestPackageUpdate(packageNames)] = inProgressPackage;
        emit inProgressRequest();
        return true;
    }
    return false;
}

bool PackageManagerTracker::requestPackageRemoval(const QString &packageName) {
    if(!packageInProgress(packageName)){
        InProgressPackage inProgressPackage;
        inProgressPackage.name = packageName;
        inProgressPackage.status = PackageStatus::REMOVING;
        inProgressPackagesMap[m_pkgMgr->requestPackageRemoval(packageName)] = inProgressPackage;
        emit inProgressRequest();
        return true;
    }
    return false;
}

bool PackageManagerTracker::requestPackageTaskCancel(const QString &packageName) {
    QUuid taskID;
    if(packageInProgress(packageName, taskID)){
        m_pkgMgr->requestTaskCancel(taskID);
        return true;
    }
    return false;
}

void PackageManagerTracker::packageInstalledHandler(const QUuid &taskId,const QString &name) {
    if (packageInProgress(taskId)) {
        inProgressPackagesMap.erase(taskId);
        emit packageInstalled(name);
        emit taskDataReceived(name,name + " package was installed successfully.");
    }
}

void PackageManagerTracker::packageRemovedHandler(const QUuid &taskId,const QString &name) {
    if (packageInProgress(taskId)) {
        inProgressPackagesMap.erase(taskId);
        emit packageRemoved(name);
        emit taskDataReceived(name,name + " package was removed successfully.");
    }
}

void PackageManagerTracker::packageUpdatedHandler(const QUuid &taskId,const QStringList &nameList) {
    if (packageInProgress(taskId)) {
        QString name = inProgressPackagesMap[taskId].name;
        inProgressPackagesMap.erase(taskId);
        emit packageUpdated(name);
        emit taskDataReceived(name,name + " package was updated successfully.");
    }
}


void PackageManagerTracker::packageTaskCanceledHandler(const QUuid &taskId) {
    if (packageInProgress(taskId)){
        QString name = inProgressPackagesMap[taskId].name;
        inProgressPackagesMap.erase(taskId);
        emit packageTaskCanceled(name);
    }
}

void PackageManagerTracker::taskFailedHandler(const QUuid &taskId, const QString &message) {
    if (packageInProgress(taskId)) {
        QString name = inProgressPackagesMap[taskId].name;
        inProgressPackagesMap.erase(taskId);
        emit progressFailed(name, message);
        emit taskDataReceived(name,"*** Failed - " + message);
    }
    emit taskFailed(taskId,message);
}

void PackageManagerTracker::taskDoneHandler(const QUuid &taskId, const QString &message) {
//    qDebug() << " - TBD taskDoneHandler";
}

void PackageManagerTracker::taskDataHandler(const QUuid &taskId, const QString &data) {
    if (packageInProgress(taskId))
        emit taskDataReceived(inProgressPackagesMap[taskId].name,data);
//    qDebug().noquote() << data.trimmed();
}

bool PackageManagerTracker::inInstalling(const QString &packageName) {
    QUuid taskId;
    if (packageInProgress(packageName,taskId)) {
        return (inProgressPackagesMap[taskId].status==PackageStatus::INSTALLING);
    }
    return false;
}

bool PackageManagerTracker::inUpdating(const QString &packageName) {
    QUuid taskId;
    if (packageInProgress(packageName,taskId)) {
        return (inProgressPackagesMap[taskId].status==PackageStatus::UPDATING);
    }
    return false;
}

bool PackageManagerTracker::inRemoving(const QString &packageName) {
    QUuid taskId;
    if (packageInProgress(packageName,taskId)) {
        return (inProgressPackagesMap[taskId].status==PackageStatus::REMOVING);
    }
    return false;
}

QVector<Package *> PackageManagerTracker::inProgressList() {
    DataAccessLayer *dbLayer = new DataAccessLayer(_dbPath);
    QVector<Package *> pkgs;
    for (const auto &l: inProgressPackagesMap) {
        auto *pkg = dbLayer->packageDetails(l.second.name);
        // if pkg not available in PX db so it's avail in `other applications` --> we will add it manually
        if(!pkg) {
            pkg = Package::MakePackage(this);
            pkg->setName(l.second.name);
            pkg->setTitle(l.second.name);
        }
        pkgs.append(pkg);
    }
    return pkgs;
}

QUuid PackageManagerTracker::requestInstalledPackageList() {
    return m_pkgMgr->requestInstalledPackages();
}

QUuid PackageManagerTracker::requestUserUpdatablePackageList() {
    return m_pkgMgr->requestUserUpgradablePackages();
}

QUuid PackageManagerTracker::requestSystemUpdatablePackageList() {
    return m_pkgMgr->requestSystemUpgradablePackages();
}

QUuid PackageManagerTracker::requestSystemUpdate(){
    return m_pkgMgr->requestSystemUpdate();
}

void PackageManagerTracker::installedPackageListHandler(const QUuid &taskId, const QVector<Package *> &packageList) {
    emit installedPackageListReady(packageList);
}

void
PackageManagerTracker::userUpdatablePackageListHandler(const QUuid &taskId, const QVector<Package *> &packageList) {
    emit userUpdatablePackageListReady(packageList);
}

void
PackageManagerTracker::systemUpdatablePackageListHandler(const QUuid &taskId, const QVector<Package *> &packageList) {
    emit systemUpdatablePackageListReady(packageList);
}