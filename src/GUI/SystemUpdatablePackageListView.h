//
// Created by hamzeh on 11/18/19.
//

#ifndef PX_SOFTWARE_SYSTEMUPDATABLEPACKAGELISTVIEW_H
#define PX_SOFTWARE_SYSTEMUPDATABLEPACKAGELISTVIEW_H
#include <QVector>

#include "PxQScrollArea.h"
#include "PackageManager.h"
#include "PackageManagerTracker.h"
#include "PackageListWidgetItem.h"

using namespace PKG;
class SystemUpdatablePackageListView : public PxQScrollArea{
    Q_OBJECT
public:
    static SystemUpdatablePackageListView *Instance();
    static void init(const int &id, const QString &title);

private slots:
    void getSystemUpdatablePackages(const QVector<Package *> &packageList);

private:
    SystemUpdatablePackageListView(bool removeEnable,const int &id, const QString &title, PxQScrollArea * parent= nullptr);
    static SystemUpdatablePackageListView *_instance;
    static bool removeEnable;
    QBoxLayout *boxLayout=nullptr;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
};



#endif //PX_SOFTWARE_SYSTEMUPDATABLEPACKAGELISTVIEW_H
