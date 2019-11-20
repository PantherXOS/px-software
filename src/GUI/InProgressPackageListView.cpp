//
// Created by hamzeh on 11/19/19.
//

#include "InProgressPackageListView.h"

InProgressPackageListView *InProgressPackageListView::_instance = nullptr;
bool InProgressPackageListView::removeEnable = false;

InProgressPackageListView *InProgressPackageListView::Instance() {
    if(_instance==nullptr){
        qCritical() << "InProgressPackageListView is not initiated";
    }
    return _instance;
}

void InProgressPackageListView::init(const int &id, const QString &title) {
    if(_instance==nullptr)
        _instance = new InProgressPackageListView(false,id,title);
}

void InProgressPackageListView::packageProgressDoneHandler(const QString &) {
    refresh();
}

InProgressPackageListView::InProgressPackageListView(bool _removeEnable, const int &id, const QString &title,
                                                     PxQScrollArea *parent) : PxQScrollArea(id,title, parent) {
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    removeEnable = _removeEnable;
    connect(m_pkgMgrTrk, SIGNAL(packageRemoved(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrk, SIGNAL(packageInstalled(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrk, SIGNAL(packageUpdated(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
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
        auto packageWidget = new PackageListWidgetItem(pkg, InProgressPackageListView::removeEnable);
        boxLayout->addWidget(packageWidget);
    }
}
