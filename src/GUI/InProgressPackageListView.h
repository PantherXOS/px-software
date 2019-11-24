//
// Created by hamzeh on 11/19/19.
//

#include "PxQScrollArea.h"
#include "PackageManager.h"
#include "PackageManagerTracker.h"
#include "PackageListWidgetItem.h"

using namespace PKG;

#ifndef PX_SOFTWARE_INPROGRESSPAKCAGELISTVIEW_H
#define PX_SOFTWARE_INPROGRESSPAKCAGELISTVIEW_H


class InProgressPackageListView : public PxQScrollArea{
Q_OBJECT
public:
    static InProgressPackageListView *Instance();
    static void init(const int &id, const QString &title);
    void refresh();

private slots:
    void packageProgressDoneHandler(const QString&);

private:
    InProgressPackageListView(bool _removeEnable, const int &id, const QString &title, PxQScrollArea * parent= nullptr);
    static InProgressPackageListView *_instance;
    static bool removeEnable;
    QBoxLayout *boxLayout=nullptr;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
};
#endif //PX_SOFTWARE_INPROGRESSPAKCAGELISTVIEW_H
