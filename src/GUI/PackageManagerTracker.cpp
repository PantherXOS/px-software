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
    if (inProgressPackagesMap.find(taskId) != inProgressPackagesMap.end())
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
}

QVector<Category *> PackageManagerTracker::categoryList() {
    return m_pkgMgr->categoryList();
}

bool PackageManagerTracker::requestPackageInstallation(const QString &packageName) {
    if(!packageInProgress(packageName) != 0) {
        InProgressPackage inProgressPackage;
        inProgressPackage.name = packageName;
        inProgressPackage.status = PackageStatus::INSTALLING;
        inProgressPackagesMap[m_pkgMgr->requestPackageInstallation(packageName)] = inProgressPackage;
        return true;
    }
    return false;
}

bool PackageManagerTracker::requestPackageUpdate(const QString &packageName) {
    if(!packageInProgress(packageName) != 0) {
        InProgressPackage inProgressPackage;
        inProgressPackage.name = packageName;
        inProgressPackage.status = PackageStatus::UPDATING;
        QStringList packageNames = {packageName};
        inProgressPackagesMap[m_pkgMgr->requestPackageUpdate(packageNames)] = inProgressPackage;
        return true;
    }
    return false;
}

bool PackageManagerTracker::requestPackageRemoval(const QString &packageName) {
    if(!packageInProgress(packageName) != 0){
        InProgressPackage inProgressPackage;
        inProgressPackage.name = packageName;
        inProgressPackage.status = PackageStatus::REMOVING;
        inProgressPackagesMap[m_pkgMgr->requestPackageRemoval(packageName)] = inProgressPackage;
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

void PackageManagerTracker::taskFailedHandler(const QUuid &taskId, const QString &message) {
    if (packageInProgress(taskId)) {
        QString name = inProgressPackagesMap[taskId].name;
        inProgressPackagesMap.erase(taskId);
        emit progressFailed(name, message);
        emit taskDataReceived(name,"*** Failed - " + message);
    }
}

void PackageManagerTracker::taskDoneHandler(const QUuid &taskId, const QString &message) {
    qDebug() << " - TBD taskDoneHandler";
}

void PackageManagerTracker::taskDataHandler(const QUuid &taskId, const QString &data) {
    if (packageInProgress(taskId))
        emit taskDataReceived(inProgressPackagesMap[taskId].name,data);
    //qDebug() << data;
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
    DataAccessLayer *dbLayer = new DataAccessLayer("./SAMPLE_DB");
    QVector<Package *> pkgs;
    for (const auto &l: inProgressPackagesMap) {
        auto *pkg = dbLayer->packageDetails(l.second.name);
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
