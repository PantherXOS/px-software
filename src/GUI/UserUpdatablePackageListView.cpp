//
// Created by hamzeh on 11/18/19.
//

#include "UserUpdatablePackageListView.h"

UserUpdatablePackageListView *UserUpdatablePackageListView::_instance = nullptr;

UserUpdatablePackageListView *UserUpdatablePackageListView::Instance() {
    if(_instance==nullptr){
        qCritical() << "UserUpdatablePackageListView is not initiated";
    }
    return _instance;
}

void UserUpdatablePackageListView::init(const QString &title) {
    if(_instance==nullptr)
        _instance = new UserUpdatablePackageListView(title, nullptr);
}

void UserUpdatablePackageListView::refresh() {
    m_pkgMgrTrk->requestUserUpdatablePackageList();
}

void UserUpdatablePackageListView::getUserUpdatablePackages(const QVector<Package *> &packageList) {
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

UserUpdatablePackageListView::UserUpdatablePackageListView(const QString &title, PxQScrollArea *parent)
        : PxQScrollArea(title, parent) {
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
    connect(m_pkgMgrTrk, SIGNAL(userUpdatablePackageListReady(
                                        const QVector<Package *> &)), this, SLOT(getUserUpdatablePackages(
                                                                                       const QVector<Package *> &)));
}
