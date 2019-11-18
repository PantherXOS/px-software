//
// Created by hamzeh on 11/18/19.
//

#include "InstalledPackageListView.h"

InstalledPackageListView *InstalledPackageListView::_instance = nullptr;
bool InstalledPackageListView::removeEnable = false;

InstalledPackageListView *InstalledPackageListView::Instance() {
    if(_instance==nullptr){
        qCritical() << "InstalledPackageListView is not initiated";
    }
    return _instance;
}


void InstalledPackageListView::init(int id, QString title) {
    if(_instance==nullptr)
        _instance = new InstalledPackageListView(true,id,title);
}


InstalledPackageListView::InstalledPackageListView(bool _removeEnable, int id,
                                                   QString title, PxQScrollArea *parent) : PxQScrollArea(id,title, parent){
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    removeEnable = _removeEnable;
    connect(m_pkgMgrTrk, SIGNAL(installedPackageListReady(
                                        const QVector<Package *>)), this, SLOT(getInstalledPackages(
                                                                                       const QVector<Package *>)));
    connect(m_pkgMgrTrk, SIGNAL(packageRemoved(const QString)),this, SLOT(packageProgressDoneHandler(const QString)));
    connect(m_pkgMgrTrk, SIGNAL(packageInstalled(const QString)),this, SLOT(packageProgressDoneHandler(const QString)));
    refresh();
}

void InstalledPackageListView::refresh(){
    m_pkgMgrTrk->requestInstalledPackageList();
}

void InstalledPackageListView::packageProgressDoneHandler(const QString &name) {
    refresh();
}

void InstalledPackageListView::getInstalledPackages(const QVector<Package *> &packageList){
    if(boxLayout!=nullptr)
        delete boxLayout;
    boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QWidget *widget=new QWidget;
    widget->setLayout(boxLayout);
    setWidgetResizable(true);
    setWidget(widget);
    for(auto pkg:packageList) {
        PackageListWidgetItem *packageWidget = new PackageListWidgetItem(pkg, InstalledPackageListView::removeEnable);
        boxLayout->addWidget(packageWidget);
    }
}


