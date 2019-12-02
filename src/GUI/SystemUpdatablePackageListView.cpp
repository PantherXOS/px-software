//
// Created by hamzeh on 11/18/19.
//

#include "SystemUpdatablePackageListView.h"

SystemUpdatablePackageListView *SystemUpdatablePackageListView::_instance = nullptr;

SystemUpdatablePackageListView *SystemUpdatablePackageListView::Instance() {
    if(_instance==nullptr){
        qCritical() << "UserUpdatablePackageListView is not initiated";
    }
    return _instance;
}

void SystemUpdatablePackageListView::init(const QString &title) {
    if(_instance==nullptr)
        _instance = new SystemUpdatablePackageListView(title, nullptr);
}

void SystemUpdatablePackageListView::getSystemUpdatablePackages(const QVector<Package *> &packageList) {
    if(boxLayout!=nullptr)
        delete boxLayout;
    boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QWidget *widget=new QWidget(this);
    widget->setLayout(boxLayout);
    setWidgetResizable(true);
    setWidget(widget);
    for(auto pkg:packageList) {
        auto packageWidget = new PackageListWidgetItem(pkg, true, this);
        boxLayout->addWidget(packageWidget);
    }
}

SystemUpdatablePackageListView::SystemUpdatablePackageListView(const QString &title,
                                                               PxQScrollArea *parent) : PxQScrollArea(title, parent) {
    QMovie *movie = new QMovie(":images/general/src/GUI/resources/loading.gif");
    QSize size(128,128);
    movie->setScaledSize(size);
    setAlignment(Qt::AlignCenter);
    QLabel *processLabel = new QLabel(this);
    processLabel->setMovie(movie);
    processLabel->setFixedSize(size);
    movie->start();
    setWidget(processLabel);

    m_pkgMgrTrk = PackageManagerTracker::Instance();
    connect(m_pkgMgrTrk, SIGNAL(systemUpdatablePackageListReady(
                                        const QVector<Package *> &)), this, SLOT(getSystemUpdatablePackages(
                                                                                       const QVector<Package *> &)));
}

void SystemUpdatablePackageListView::refresh() {
    m_pkgMgrTrk->requestSystemUpdatablePackageList();
}
