//
// Created by hamzeh on 11/18/19.
//

#ifndef PX_SOFTWARE_USERUPDATABLEPACKAGELISTVIEW_H
#define PX_SOFTWARE_USERUPDATABLEPACKAGELISTVIEW_H
#include <QVector>
#include <QMovie>

#include "PxQScrollArea.h"
#include "PackageManager.h"
#include "PackageManagerTracker.h"
#include "PackageListWidgetItem.h"

using namespace PKG;

class UserUpdatablePackageListView : public PxQScrollArea{
    Q_OBJECT
public:
    static UserUpdatablePackageListView *Instance();
    static void init(const QString &title);
    void refresh();

private slots:
    void getUserUpdatablePackages(const QVector<Package *> &packageList);
    void taskFailedHandler(const QUuid &, const QString&);

private:
    UserUpdatablePackageListView(const QString &title, PxQScrollArea *parent);
    static UserUpdatablePackageListView *_instance;
    QBoxLayout *boxLayout=nullptr;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
    QUuid taskId;
};


#endif //PX_SOFTWARE_USERUPDATABLEPACKAGELISTVIEW_H
