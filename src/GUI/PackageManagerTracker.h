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
#include "PackageListWidgetItem.h"

using namespace PKG;
class PackageManagerTracker : public QObject{
Q_OBJECT
public:
    static PackageManagerTracker &Instance();
    QUuid requestPackageInstallation(const QString &packageName);
    QUuid requestPackageUpdate(const QStringList &packageNameList);
    QUuid requestPackageRemoval(const QString &packageName);

private slots:
    void packagedInstalledHandler(const QString &name);
    void packagedRemovedHandler(const QString &name);
    void packagedUpdatedHandler(const QStringList &nameList);
    void taskFailedHandler(const QUuid &, const QString &message);
    void taskDoneHandler(const QUuid &, const QString &message);
    void taskDataHandler(const QUuid &taskId, const QString &data);

signals:
    void packageInstalled(const QString &name);
    void packageUpdated(const QString &name);
    void packageRemoved(const QString &name);
    void progressFailed(const QString &name);


private:
    PackageManagerTracker();
    PackageManager *m_pkgMgr = nullptr;
    QMap<QUuid , QString> inProgressPackages;
};


#endif //PX_SOFTWARE_PACKAGEMANAGERTRACKER_H
