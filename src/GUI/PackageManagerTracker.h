//
// Created by hamzeh on 11/12/19.
//

#ifndef PX_SOFTWARE_PACKAGEMANAGERTRACKER_H
#define PX_SOFTWARE_PACKAGEMANAGERTRACKER_H

#include <QString>
#include <QStringList>
#include <QDebug>
#include <QObject>
#include <QMap>

#include "PackageManager.h"
#include "DataEntities.h"
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
    QUuid requestPackageInstallation(const QString &packageName);
    QUuid requestPackageUpdate(const QString &packageName);
    QUuid requestPackageRemoval(const QString &packageName);
    bool packageInProgress(const QString &packageName, QUuid &taskId);
    bool packageInProgress(const QUuid &taskId);
    bool inInstalling(const QString &packageName);
    bool inRemoving(const QString &packageName);
    bool inUpdating(const QString &packageName);
    QStringList getList();

private slots:
    void packageInstalledHandler(const QUuid &taskId,const QString &name);
    void packageRemovedHandler(const QUuid &taskId,const QString &name);
    void packageUpdatedHandler(const QUuid &taskId,const QStringList &nameList);
    void taskFailedHandler(const QUuid &, const QString &message);
    void taskDoneHandler(const QUuid &, const QString &message);
    void taskDataHandler(const QUuid &taskId, const QString &data);

signals:
    void packageInstalled(const QString &name);
    void packageUpdated(const QString &name);
    void packageRemoved(const QString &name);
    void progressFailed(const QString &name, const QString &message);


private:
    PackageManagerTracker();
    static PackageManagerTracker *_instance;
    PackageManager *m_pkgMgr = nullptr;
    map<QUuid , InProgressPackage> inProgressPackagesMap;
};


#endif //PX_SOFTWARE_PACKAGEMANAGERTRACKER_H
