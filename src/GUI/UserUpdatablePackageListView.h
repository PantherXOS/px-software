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

using namespace PKG;

class UserUpdatablePackageListView : public PxQScrollArea{
    Q_OBJECT
public:
    static UserUpdatablePackageListView *Instance();
    static void init(int id, const QString &title);

private slots:
    void getUserUpdatablePackages(const QVector<Package *> &packageList);

private:
    UserUpdatablePackageListView(bool _removeEnable, const int id, const QString &title, PxQScrollArea *parent = nullptr);
    static UserUpdatablePackageListView *_instance;
    static bool removeEnable;
    QBoxLayout *boxLayout=nullptr;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
};


#endif //PX_SOFTWARE_USERUPDATABLEPACKAGELISTVIEW_H
