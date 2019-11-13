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
    for(auto p : inProgressPackages)
        if(p.second == packageName) {
            taskId = p.first;
            return true;
        }
    return false;
}

bool PackageManagerTracker::packageInProgress(const QUuid &taskId) {
    if (inProgressPackages.find(taskId) != inProgressPackages.end())
        return true;
    return false;
}

PackageManagerTracker::PackageManagerTracker(){
    m_pkgMgr = PKG::PackageManager::Instance();
    connect(m_pkgMgr, SIGNAL(packageInstalled(const QString &)),this, SLOT(packagedInstalledHandler(const QString &)));
    connect(m_pkgMgr, SIGNAL(packageUpdated(const QStringList &)),this, SLOT(packagedUpdatedHandler(const QStringList &)));
    connect(m_pkgMgr, SIGNAL(packageRemoved(const QString &)),this, SLOT(packagedRemovedHandler(const QString &)));
    connect(m_pkgMgr, SIGNAL(taskFailed(const QUuid &, const QString &)),this, SLOT(taskFailedHandler(const QUuid &, const QString &)));
    connect(m_pkgMgr, SIGNAL(newTaskData(const QUuid &, const QString &)), this, SLOT(taskDataHandler(const QUuid &, const QString &)));
    connect(m_pkgMgr, SIGNAL(taskDone(const QUuid &, const QString &)), this, SLOT(taskDoneHandler(const QUuid &, const QString &)));
}

QUuid PackageManagerTracker::requestPackageInstallation(const QString &packageName) {
    QUuid taskId;
    if(!packageInProgress(packageName, taskId) != 0){
        QUuid uuid = m_pkgMgr->requestPackageInstallation(packageName);
        inProgressPackages[uuid] = packageName;
        return uuid;
    }
    return taskId; // TODO
}

QUuid PackageManagerTracker::requestPackageUpdate(const QString &packageName) {
    QUuid taskId;
    if(!packageInProgress(packageName, taskId) != 0) {
        QStringList packageNames = {packageName};
        QUuid uuid = m_pkgMgr->requestPackageUpdate(packageNames);
        inProgressPackages[uuid] = packageName;
        return uuid;
    }
    return taskId;
}

QUuid PackageManagerTracker::requestPackageRemoval(const QString &packageName) {
    QUuid taskId;
    if(!packageInProgress(packageName, taskId) != 0) {
        QUuid uuid = m_pkgMgr->requestPackageRemoval(packageName);
        inProgressPackages[uuid] = packageName;
        return uuid;
    }
    return taskId;
}

void PackageManagerTracker::packagedInstalledHandler(const QString &name) {
    
}

void PackageManagerTracker::packagedRemovedHandler(const QString &name) {

}

void PackageManagerTracker::packagedUpdatedHandler(const QStringList &nameList) {

}

void PackageManagerTracker::taskFailedHandler(const QUuid &taskId, const QString &message) {
    if (inProgressPackages.find(taskId) != inProgressPackages.end()) {
        emit progressFailed(inProgressPackages[taskId]);
        inProgressPackages.erase(taskId);
    }
}

void PackageManagerTracker::taskDoneHandler(const QUuid &taskId, const QString &message) {
    qDebug() << " - TBD taskDoneHandler";
    if (inProgressPackages.find(taskId) != inProgressPackages.end()) {
//        emit progressFailed(inProgressPackages[uuid]);
        inProgressPackages.erase(taskId);
    }
}

void PackageManagerTracker::taskDataHandler(const QUuid &taskId, const QString &data) {
    qDebug() << " - TBD Embedded Terminal";
}