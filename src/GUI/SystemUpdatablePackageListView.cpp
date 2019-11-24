//
// Created by hamzeh on 11/18/19.
//

#include "SystemUpdatablePackageListView.h"

SystemUpdatablePackageListView *SystemUpdatablePackageListView::_instance = nullptr;
bool SystemUpdatablePackageListView::removeEnable = false;

SystemUpdatablePackageListView *SystemUpdatablePackageListView::Instance() {
    if(_instance==nullptr){
        qCritical() << "UserUpdatablePackageListView is not initiated";
    }
    return _instance;
}

void SystemUpdatablePackageListView::init(const int &id, const QString &title) {
    if(_instance==nullptr)
        _instance = new SystemUpdatablePackageListView(true,id,title);
}

void SystemUpdatablePackageListView::getSystemUpdatablePackages(const QVector<Package *> &packageList) {
    if(boxLayout!=nullptr)
        delete boxLayout;
    boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QWidget *widget=new QWidget;
    widget->setLayout(boxLayout);
    setWidgetResizable(true);
    setWidget(widget);
    for(auto pkg:packageList) {
        auto packageWidget = new PackageListWidgetItem(pkg, SystemUpdatablePackageListView::removeEnable);
        boxLayout->addWidget(packageWidget);
    }
}

SystemUpdatablePackageListView::SystemUpdatablePackageListView(bool _removeEnable, const int &id, const QString &title,
                                                               PxQScrollArea *parent) : PxQScrollArea(id,title, parent){
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    removeEnable = _removeEnable;
    connect(m_pkgMgrTrk, SIGNAL(systemUpdatablePackageListReady(
                                        const QVector<Package *> &)), this, SLOT(getSystemUpdatablePackages(
                                                                                       const QVector<Package *> &)));
    m_pkgMgrTrk->requestSystemUpdatablePackageList();
}
