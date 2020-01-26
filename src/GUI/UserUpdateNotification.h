//
// Created by hamzeh on 1/6/20.
//

#ifndef PX_SOFTWARE_USERUPDATENOTIFICATION_H
#define PX_SOFTWARE_USERUPDATENOTIFICATION_H
#include <QObject>
#include <lxqt/LXQt/Notification>

#include "PackageManagerTracker.h"

class UserUpdateNotification : public QObject {
    Q_OBJECT

public:
    static UserUpdateNotification &instance(){
        static UserUpdateNotification _instance;
        return _instance;
    }

private slots:
    void userUpdatablePackageListHandler(const QUuid &, const QVector<Package *> packageList) {
        if(packageList.size())
            LXQt::Notification::notify("(" + QString::number(packageList.size()) + ") "+ tr("User Upgradable Packages are available."));
        disconnect(userUpdatablePackageSignalConnection);
    };

private:
    UserUpdateNotification(){
        m_pkgMgr = PKG::PackageManager::Instance();
        userUpdatablePackageSignalConnection = connect(m_pkgMgr, SIGNAL(userUpgradablePackagesReady(
                                         const QUuid &,
                                         const QVector<Package *>)), this, SLOT(userUpdatablePackageListHandler(
                                                                                        const QUuid &, const QVector<Package *>)));
    }

    PackageManager *m_pkgMgr = nullptr;
    QMetaObject::Connection userUpdatablePackageSignalConnection;

};
#endif //PX_SOFTWARE_USERUPDATENOTIFICATION_H
