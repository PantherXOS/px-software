//
// Created by hamzeh on 11/19/19.
//

#include "InProgressPakcageListView.h"

InProgressPakcageListView *InProgressPakcageListView::_instance = nullptr;
bool InProgressPakcageListView::removeEnable = false;
//void InProgressPakcageListView::refresh();

InProgressPakcageListView *InProgressPakcageListView::Instance() {
    if(_instance==nullptr){
        qCritical() << "InProgressPakcageListView is not initiated";
    }
    return _instance;
}

void InProgressPakcageListView::init(const int &id, const QString &title) {
    if(_instance==nullptr)
        _instance = new InProgressPakcageListView(false,id,title);
}

void InProgressPakcageListView::packageProgressDoneHandler(const QString &) {
    refresh();
}

InProgressPakcageListView::InProgressPakcageListView(bool _removeEnable, const int &id, const QString &title,
                                                     PxQScrollArea *parent) : PxQScrollArea(id,title, parent) {
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    removeEnable = _removeEnable;
    connect(m_pkgMgrTrk, SIGNAL(packageRemoved(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrk, SIGNAL(packageInstalled(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrk, SIGNAL(packageUpdated(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
}

void InProgressPakcageListView::refresh() {
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
        auto packageWidget = new PackageListWidgetItem(pkg, InProgressPakcageListView::removeEnable);
        boxLayout->addWidget(packageWidget);
    }
}
