//
// Created by hamzeh on 11/18/19.
//

#ifndef PX_SOFTWARE_INSTALLEDPACKAGELISTVIEW_H
#define PX_SOFTWARE_INSTALLEDPACKAGELISTVIEW_H
#include <QVector>
#include <QMovie>

#include "src/GUI/px-qobjects/PxQScrollArea.h"
#include "PackageManager.h"
#include "PackageManagerTracker.h"
#include "PackageListWidgetItem.h"
#include "src/GUI/px-qobjects/PxViewLoadingAnimation.h"
#include "Settings.h"

using namespace PKG;

class InstalledPackageListView : public PxQScrollArea{
    Q_OBJECT
public:
    static InstalledPackageListView *Instance();
    static void init(const QString &title);
    void refresh();

private slots:
    void getInstalledPackages(const QVector<Package *> &packageList);
    void packageProgressDoneHandler(const QString&);
    void taskFailedHandler(const QUuid &, const QString&);

private:
    InstalledPackageListView(const QString &title, PxQScrollArea *parent);
    static InstalledPackageListView *_instance;
    QBoxLayout *boxLayout=nullptr;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
    QUuid taskId;
};


#endif //PX_SOFTWARE_INSTALLEDPACKAGELISTVIEW_H
