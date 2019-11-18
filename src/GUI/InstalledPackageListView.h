//
// Created by hamzeh on 11/18/19.
//

#ifndef PX_SOFTWARE_INSTALLEDPACKAGELISTVIEW_H
#define PX_SOFTWARE_INSTALLEDPACKAGELISTVIEW_H
#include <QVector>

#include "PxQScrollArea.h"
#include "PackageManager.h"
#include "PackageManagerTracker.h"
#include "PackageListWidgetItem.h"

using namespace PKG;

class InstalledPackageListView : PxQScrollArea{
    Q_OBJECT
public:
    static InstalledPackageListView *Instance();
    static void init(int id, QString title);

private slots:
    void getInstalledPackages(const QVector<Package *> &packageList);
    void packageProgressDoneHandler(const QString&);

private:
    InstalledPackageListView(bool removeEnable, int id, QString title, PxQScrollArea * parent= nullptr);
    void refresh();
    static InstalledPackageListView *_instance;
    static QVector<Package *> packageList;
    static bool removeEnable;
    QBoxLayout *boxLayout=nullptr;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
};


#endif //PX_SOFTWARE_INSTALLEDPACKAGELISTVIEW_H
