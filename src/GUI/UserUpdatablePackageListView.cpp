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
    auto loading = new PxViewLoadingAnimation(this);
    setAlignment(Qt::AlignCenter);
    setWidget(loading);
    taskId = m_pkgMgrTrk->requestUserUpdatablePackageList();
}

void UserUpdatablePackageListView::getUserUpdatablePackages(const QVector<Package *> &packageList) {
    boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QWidget *widget=new QWidget(this);
    widget->setLayout(boxLayout);
    setWidgetResizable(true);
    setWidget(widget);
    if(packageList.size()){
        for(auto pkg:packageList) {
            auto packageWidget = new PackageListWidgetItem(pkg, true, this);
            boxLayout->addWidget(packageWidget);
        }
    } else {
        auto emptyLabel = new QLabel;
        emptyLabel->setText("Everything is up to date.");
        emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
        boxLayout->addWidget(emptyLabel);
    }
}

UserUpdatablePackageListView::UserUpdatablePackageListView(const QString &title, PxQScrollArea *parent)
        : PxQScrollArea(title, parent) {
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    connect(m_pkgMgrTrk, SIGNAL(userUpdatablePackageListReady(
                                        const QVector<Package *> &)), this, SLOT(getUserUpdatablePackages(
                                                                                       const QVector<Package *> &)));
    connect(m_pkgMgrTrk, SIGNAL(taskFailed(const QUuid &,const QString &)),this, SLOT(taskFailedHandler(const QUuid &,const QString &)));
}

void UserUpdatablePackageListView::taskFailedHandler(const QUuid &_taskId, const QString &message) {
    if(_taskId == taskId){
        auto emptyLabel = new QLabel;
        emptyLabel->setText(message);
        emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
        boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        boxLayout->addWidget(emptyLabel);
        QWidget *widget=new QWidget;
        widget->setLayout(boxLayout);
        setWidgetResizable(true);
        setWidget(widget);
    }
}