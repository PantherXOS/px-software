//
// Created by hamzeh on 11/19/19.
//

#include "InProgressPackageListView.h"

InProgressPackageListView *InProgressPackageListView::_instance = nullptr;

InProgressPackageListView *InProgressPackageListView::Instance() {
    if(_instance==nullptr){
        qCritical() << "InProgressPackageListView is not initiated";
    }
    return _instance;
}

void InProgressPackageListView::init(const QString &title) {
    if(_instance==nullptr)
        _instance = new InProgressPackageListView(title, nullptr);
}

void InProgressPackageListView::packageProgressDoneHandler(const QString &packageName) {
    refresh();
}

void InProgressPackageListView::packageProgressDoneHandler(const QString &packageName,const QString &message) {
    refresh();
}

InProgressPackageListView::InProgressPackageListView(const QString &title, PxQScrollArea *parent)
        : PxQScrollArea(title, parent) {
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    connect(m_pkgMgrTrk, SIGNAL(packageRemoved(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrk, SIGNAL(packageInstalled(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrk, SIGNAL(packageUpdated(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrk, SIGNAL(packageTaskCanceled(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrk, SIGNAL(progressFailed(const QString &,const QString&)),this, SLOT(packageProgressDoneHandler(const QString &, const QString&)));
}

void InProgressPackageListView::refresh() {
    QVector<Package *> pkgs = m_pkgMgrTrk->inProgressList();
    if(boxLayout!=nullptr)
        delete boxLayout;
    boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QWidget *widget=new QWidget;
    widget->setLayout(boxLayout);
    setWidgetResizable(true);
    setWidget(widget);
    for(auto pkg:pkgs) {
        auto packageWidget = new PackageListWidgetItem(pkg, false, this);
        boxLayout->addWidget(packageWidget);
    }
}
