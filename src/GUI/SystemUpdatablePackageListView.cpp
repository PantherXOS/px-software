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
        emptyLabel->setFont(QFont("default", 16));
        boxLayout->addWidget(emptyLabel);
    }
}

SystemUpdatablePackageListView::SystemUpdatablePackageListView(const QString &title,
                                                               PxQScrollArea *parent) : PxQScrollArea(title, parent) {
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    connect(m_pkgMgrTrk, SIGNAL(systemUpdatablePackageListReady(
                                        const QVector<Package *> &)), this, SLOT(getSystemUpdatablePackages(
                                                                                       const QVector<Package *> &)));
    connect(m_pkgMgrTrk, SIGNAL(taskFailed(const QUuid &,const QString &)),this, SLOT(taskFailedHandler(const QUuid &,const QString &)));
}

void SystemUpdatablePackageListView::refresh() {
    QMovie *movie = new QMovie(":images/general/src/GUI/resources/loading.gif");
    QSize size(128,128);
    movie->setScaledSize(size);
    setAlignment(Qt::AlignCenter);
    QLabel *processLabel = new QLabel(this);
    processLabel->setMovie(movie);
    processLabel->setFixedSize(size);
    movie->start();
    setWidget(processLabel);
    taskId = m_pkgMgrTrk->requestSystemUpdatablePackageList();
}

void SystemUpdatablePackageListView::taskFailedHandler(const QUuid & _taskId, const QString &message) {
    if(_taskId == taskId){
        auto emptyLabel = new QLabel;
        emptyLabel->setText(message);
        emptyLabel->setFont(QFont("default", 16));
        boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        boxLayout->addWidget(emptyLabel);
        QWidget *widget=new QWidget;
        widget->setLayout(boxLayout);
        setWidgetResizable(true);
        setWidget(widget);
    }
}