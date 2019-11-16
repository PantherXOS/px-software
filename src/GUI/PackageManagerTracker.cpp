//
// Created by hamzeh on 11/12/19.
//

#include "PackageManagerTracker.h"

PackageManagerTracker *PackageManagerTracker::_instance = nullptr;

PackageManagerTracker *PackageManagerTracker::Instance() {
    if(PackageManagerTracker::_instance == nullptr)
        PackageManagerTracker::_instance = new PackageManagerTracker;
    return PackageManagerTracker::_instance;
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
    if (inProgressPackagesMap.find(taskId) != inProgressPackagesMap.end())
        return true;
    return false;
}

PackageManagerTracker::PackageManagerTracker(){
    m_pkgMgr = PKG::PackageManager::Instance();
    connect(m_pkgMgr, SIGNAL(packageInstalled(const QUuid &,const QString &)),this, SLOT(packageInstalledHandler(const QUuid &,const QString &)));
    connect(m_pkgMgr, SIGNAL(packageUpdated(const QUuid &,const QStringList &)),this, SLOT(packageUpdatedHandler(const QUuid &,const QStringList &)));
    connect(m_pkgMgr, SIGNAL(packageRemoved(const QUuid &,const QString &)),this, SLOT(packageRemovedHandler(const QUuid &,const QString &)));
    connect(m_pkgMgr, SIGNAL(taskFailed(const QUuid &, const QString &)),this, SLOT(taskFailedHandler(const QUuid &, const QString &)));
    connect(m_pkgMgr, SIGNAL(newTaskData(const QUuid &, const QString &)), this, SLOT(taskDataHandler(const QUuid &, const QString &)));
    connect(m_pkgMgr, SIGNAL(taskDone(const QUuid &, const QString &)), this, SLOT(taskDoneHandler(const QUuid &, const QString &)));
}

QUuid PackageManagerTracker::requestPackageInstallation(const QString &packageName) {
    QUuid taskId;
    if(!packageInProgress(packageName, taskId) != 0){
        InProgressPackage inProgressPackage;
        inProgressPackage.name = packageName;
        inProgressPackage.status = PackageStatus::INSTALLING;
        inProgressPackagesMap[taskId=m_pkgMgr->requestPackageInstallation(packageName)] = inProgressPackage;
    }
    return taskId; // TODO
}

QUuid PackageManagerTracker::requestPackageUpdate(const QString &packageName) {
    QUuid taskId;
    if(!packageInProgress(packageName, taskId) != 0) {
        InProgressPackage inProgressPackage;
        inProgressPackage.name = packageName;
        inProgressPackage.status = PackageStatus::UPDATING;
        QStringList packageNames = {packageName};
        inProgressPackagesMap[taskId=m_pkgMgr->requestPackageUpdate(packageNames)] = inProgressPackage;
    }
    return taskId;
}

QUuid PackageManagerTracker::requestPackageRemoval(const QString &packageName) {
    QUuid taskId;
    if(!packageInProgress(packageName, taskId) != 0){
        InProgressPackage inProgressPackage;
        inProgressPackage.name = packageName;
        inProgressPackage.status = PackageStatus::REMOVING;
        inProgressPackagesMap[taskId=m_pkgMgr->requestPackageRemoval(packageName)] = inProgressPackage;
    }
    return taskId;
}

void PackageManagerTracker::packageInstalledHandler(const QUuid &taskId,const QString &name) {
    if (packageInProgress(taskId)) {
        emit packageInstalled(inProgressPackagesMap[taskId].name);
        inProgressPackagesMap.erase(taskId);
        qDebug() << taskId;
    }
}

void PackageManagerTracker::packageRemovedHandler(const QUuid &taskId,const QString &name) {
    if (packageInProgress(taskId)) {
        emit packageRemoved(inProgressPackagesMap[taskId].name);
        inProgressPackagesMap.erase(taskId);
        qDebug() << taskId;
    }
}

void PackageManagerTracker::packageUpdatedHandler(const QUuid &taskId,const QStringList &nameList) {
    if (packageInProgress(taskId)) {
        emit packageUpdated(inProgressPackagesMap[taskId].name);
        inProgressPackagesMap.erase(taskId);
        qDebug() << taskId;
    }
}

void PackageManagerTracker::taskFailedHandler(const QUuid &taskId, const QString &message) {
    if (packageInProgress(taskId)) {
        emit progressFailed(inProgressPackagesMap[taskId].name, message);
        inProgressPackagesMap.erase(taskId);
        qDebug() << taskId;
    }
}

void PackageManagerTracker::taskDoneHandler(const QUuid &taskId, const QString &message) {
    qDebug() << " - TBD taskDoneHandler";
}

void PackageManagerTracker::taskDataHandler(const QUuid &taskId, const QString &data) {
    if (packageInProgress(taskId))
        emit taskDataReceived(inProgressPackagesMap[taskId].name,data);
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

QStringList PackageManagerTracker::getList() {
    map<QUuid, InProgressPackage> inProgressMap = this->inProgressPackagesMap;
    QStringList list;
    for(const auto &p : inProgressMap)
        list.append(p.second.name);
    return list;
}
