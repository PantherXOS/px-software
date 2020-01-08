//
// Created by hamzeh on 11/18/19.
//

#ifndef PX_SOFTWARE_SYSTEMUPDATABLEPACKAGELISTVIEW_H
#define PX_SOFTWARE_SYSTEMUPDATABLEPACKAGELISTVIEW_H
#include <QVector>
#include <QMovie>

#include "PxQWidget.h"
#include "PxQScrollArea.h"
#include "PackageManager.h"
#include "PackageManagerTracker.h"
#include "PackageListWidgetItem.h"
#include "PxViewLoadingAnimation.h"

using namespace PKG;
class SystemUpdatablePackageListView : public PxQScrollArea{
    Q_OBJECT
public:
    static SystemUpdatablePackageListView *Instance();
    static void init(const QString &title);
    void refresh();

private slots:
    void getSystemUpdatablePackages(const QVector<Package *> &packageList);
    void taskFailedHandler(const QUuid &, const QString&);

private:
    SystemUpdatablePackageListView(const QString &title, PxQScrollArea *parent = nullptr);
    static SystemUpdatablePackageListView *_instance;
    QBoxLayout *boxLayout=nullptr;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
    QUuid taskId;
};



#endif //PX_SOFTWARE_SYSTEMUPDATABLEPACKAGELISTVIEW_H
