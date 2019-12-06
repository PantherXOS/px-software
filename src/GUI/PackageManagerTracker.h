//
// Created by hamzeh on 11/12/19.
//

#ifndef PX_SOFTWARE_PACKAGEMANAGERTRACKER_H
#define PX_SOFTWARE_PACKAGEMANAGERTRACKER_H

#include <iostream>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QObject>
#include <QMap>

#include "PackageManager.h"
#include "DataEntities.h"

using namespace PKG;
using namespace std;
class PackageManagerTracker : public QObject{
Q_OBJECT
public:
    enum PackageStatus{
        NOTHING,
        INSTALLING,
        REMOVING,
        UPDATING,
    };

    class InProgressPackage{
    public:
        QString name;
        PackageStatus status;
    };

    static PackageManagerTracker *Instance();
    QVector<Category *> categoryList();
    QVector<Package *> inProgressList();
    QUuid requestInstalledPackageList();
    QUuid requestUserUpdatablePackageList();
    QUuid requestSystemUpdatablePackageList();
    bool requestPackageInstallation(const QString &packageName);
    bool requestPackageUpdate(const QString &packageName);
    bool requestPackageRemoval(const QString &packageName);
    bool requestPackageTaskCancel(const QString &packageName);
    bool packageInProgress(const QString &packageName);
    bool inInstalling(const QString &packageName);
    bool inRemoving(const QString &packageName);
    bool inUpdating(const QString &packageName);

private slots:
    void installedPackageListHandler(const QUuid &taskId, const QVector<Package *> &packageList);
    void userUpdatablePackageListHandler(const QUuid &taskId, const QVector<Package *> &packageList);
    void systemUpdatablePackageListHandler(const QUuid &taskId, const QVector<Package *> &packageList);
    void packageInstalledHandler(const QUuid &taskId,const QString &name);
    void packageRemovedHandler(const QUuid &taskId,const QString &name);
    void packageUpdatedHandler(const QUuid &taskId,const QStringList &nameList);
    void packageTaskCanceledHandler(const QUuid &taskId);
    void taskFailedHandler(const QUuid &, const QString &message);
    void taskDoneHandler(const QUuid &, const QString &message);
    void taskDataHandler(const QUuid &taskId, const QString &data);

signals:
    void installedPackageListReady(const QVector<Package *> &packageList);
    void userUpdatablePackageListReady(const QVector<Package *> &packageList);
    void systemUpdatablePackageListReady(const QVector<Package *> &packageList);
    void packageInstalled(const QString &name);
    void packageUpdated(const QString &name);
    void packageRemoved(const QString &name);
    void packageTaskCanceled(const QString &name);
    void progressFailed(const QString &name, const QString &message);
    void taskDataReceived(const QString &name, const QString &data);


private:
    bool packageInProgress(const QString &packageName, QUuid &taskId);
    bool packageInProgress(const QUuid &taskId);
    PackageManagerTracker();
    static PackageManagerTracker *_instance;
    PackageManager *m_pkgMgr = nullptr;
    map<QUuid , InProgressPackage> inProgressPackagesMap;
};


#endif //PX_SOFTWARE_PACKAGEMANAGERTRACKER_H
