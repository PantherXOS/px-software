//
// Created by hamzeh on 11/18/19.
//

#ifndef PX_SOFTWARE_USERUPDATABLEPACKAGELISTVIEW_H
#define PX_SOFTWARE_USERUPDATABLEPACKAGELISTVIEW_H
#include <QVector>

#include "PxQScrollArea.h"
#include "PackageManager.h"
#include "PackageManagerTracker.h"
#include "PackageListWidgetItem.h"

class UserUpdatablePackageListView : PxQScrollArea{
    Q_OBJECT
public:
    static UserUpdatablePackageListView *Instance();
    static void init(int id, QString title);

private slots:
    void getUserUpdatablePackages(const QVector<Package *> &packageList);

private:
    UserUpdatablePackageListView(bool removeEnable, int id, QString title, PxQScrollArea * parent= nullptr);
    static UserUpdatablePackageListView *_instance;
    static QVector<Package *> packageList;
    static bool removeEnable;
    QBoxLayout *boxLayout=nullptr;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
};


#endif //PX_SOFTWARE_USERUPDATABLEPACKAGELISTVIEW_H
