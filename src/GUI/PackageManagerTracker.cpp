//
// Created by hamzeh on 11/12/19.
//

#include "PackageManagerTracker.h"

PackageManagerTracker &PackageManagerTracker::Instance() {
    static PackageManagerTracker _instance;
    return _instance;
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
    QUuid uuid = m_pkgMgr->requestPackageInstallation(packageName);
    inProgressPackages[uuid] = packageName;
    return uuid;
}

QUuid PackageManagerTracker::requestPackageUpdate(const QStringList &packageNameList) {
    QUuid uuid = m_pkgMgr->requestPackageUpdate(packageNameList);
    for(auto packageName:packageNameList)
        inProgressPackages[uuid] = packageName;
    return uuid;
}

QUuid PackageManagerTracker::requestPackageRemoval(const QString &packageName) {
    QUuid uuid = m_pkgMgr->requestPackageRemoval(packageName);
    inProgressPackages[uuid] = packageName;
    return uuid;
}

void PackageManagerTracker::packagedInstalledHandler(const QString &name) {
    
}

void PackageManagerTracker::packagedRemovedHandler(const QString &name) {

}

void PackageManagerTracker::packagedUpdatedHandler(const QStringList &nameList) {

}

void PackageManagerTracker::taskFailedHandler(const QUuid &uuid, const QString &message) {
    for(auto _pkg : inProgressPackages){
        qDebug() << _pkg;
    }
}

void PackageManagerTracker::taskDoneHandler(const QUuid &, const QString &message) {

}

void PackageManagerTracker::taskDataHandler(const QUuid &taskId, const QString &data) {

}