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
    connect(m_pkgMgr, SIGNAL(packageInstalled(const QUuid &,const QString &)),this, SLOT(packageInstalledHandler(const QUuid &,const QString &)));
    connect(m_pkgMgr, SIGNAL(packageUpdated(const QUuid &,const QStringList &)),this, SLOT(packageUpdatedHandler(const QUuid &,const QStringList &)));
    connect(m_pkgMgr, SIGNAL(packageRemoved(const QUuid &,const QString &)),this, SLOT(packageRemovedHandler(const QUuid &,const QString &)));
    connect(m_pkgMgr, SIGNAL(taskFailed(const QUuid &, const QString &)),this, SLOT(taskFailedHandler(const QUuid &, const QString &)));
    connect(m_pkgMgr, SIGNAL(newTaskData(const QUuid &, const QString &)), this, SLOT(taskDataHandler(const QUuid &, const QString &)));
    connect(m_pkgMgr, SIGNAL(taskDone(const QUuid &, const QString &)), this, SLOT(taskDoneHandler(const QUuid &, const QString &)));
}

QUuid PackageManagerTracker::requestPackageInstallation(const QString &packageName) {
    QUuid taskId;
    if(!packageInProgress(packageName, taskId) != 0)
        inProgressPackages[taskId=m_pkgMgr->requestPackageInstallation(packageName)] = packageName;
    return taskId; // TODO
}

QUuid PackageManagerTracker::requestPackageUpdate(const QString &packageName) {
    QUuid taskId;
    if(!packageInProgress(packageName, taskId) != 0) {
        QStringList packageNames = {packageName};
        inProgressPackages[taskId=m_pkgMgr->requestPackageUpdate(packageNames)] = packageName;
    }
    return taskId;
}

QUuid PackageManagerTracker::requestPackageRemoval(const QString &packageName) {
    QUuid taskId;
    if(!packageInProgress(packageName, taskId) != 0)
        inProgressPackages[taskId=m_pkgMgr->requestPackageRemoval(packageName)] = packageName;
    return taskId;
}

void PackageManagerTracker::packageInstalledHandler(const QUuid &taskId,const QString &name) {
    qDebug() << "packageInstalledHandler";
    if (inProgressPackages.find(taskId) != inProgressPackages.end()) {
        emit packageInstalled(inProgressPackages[taskId]);
        inProgressPackages.erase(taskId);
    }
}

void PackageManagerTracker::packageRemovedHandler(const QUuid &taskId,const QString &name) {
    qDebug() << "packageRemovedHandler";
    if (inProgressPackages.find(taskId) != inProgressPackages.end()) {
        emit packageRemoved(inProgressPackages[taskId]);
        inProgressPackages.erase(taskId);
    }
}

void PackageManagerTracker::packageUpdatedHandler(const QUuid &taskId,const QStringList &nameList) {
    qDebug() << "packageUpdatedHandler";
    if (inProgressPackages.find(taskId) != inProgressPackages.end()) {
        emit packageUpdated(inProgressPackages[taskId]);
        inProgressPackages.erase(taskId);
    }
}

void PackageManagerTracker::taskFailedHandler(const QUuid &taskId, const QString &message) {
    if (inProgressPackages.find(taskId) != inProgressPackages.end()) {
        emit progressFailed(inProgressPackages[taskId], message);
        inProgressPackages.erase(taskId);
    }
}

void PackageManagerTracker::taskDoneHandler(const QUuid &taskId, const QString &message) {
    qDebug() << " - TBD taskDoneHandler";
}

void PackageManagerTracker::taskDataHandler(const QUuid &taskId, const QString &data) {
    qDebug() << " - TBD Embedded Terminal";
}