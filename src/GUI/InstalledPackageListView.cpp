//
// Created by hamzeh on 11/18/19.
//

#include "InstalledPackageListView.h"

InstalledPackageListView *InstalledPackageListView::_instance = nullptr;

InstalledPackageListView *InstalledPackageListView::Instance() {
    if(_instance==nullptr){
        qCritical() << "InstalledPackageListView is not initiated";
    }
    return _instance;
}


void InstalledPackageListView::init(const QString &title) {
    if(_instance==nullptr)
        _instance = new InstalledPackageListView(title, nullptr);
}


InstalledPackageListView::InstalledPackageListView(const QString &title, PxQScrollArea *parent)
        : PxQScrollArea(title,
                        parent) {
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    connect(m_pkgMgrTrk, SIGNAL(installedPackageListReady(
                                        const QVector<Package *> &)), this, SLOT(getInstalledPackages(
                                                                                       const QVector<Package *> &)));
    connect(m_pkgMgrTrk, SIGNAL(packageRemoved(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrk, SIGNAL(packageInstalled(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrk, SIGNAL(taskFailed(const QUuid &,const QString &)),this, SLOT(taskFailedHandler(const QUuid &,const QString &)));
}

void InstalledPackageListView::refresh(){
    auto loadingAnim = new PxViewLoadingAnimation(this);
    setAlignment(Qt::AlignCenter);
    setWidget(loadingAnim);
    taskId = m_pkgMgrTrk->requestInstalledPackageList();
}

void InstalledPackageListView::packageProgressDoneHandler(const QString &name) {
    refresh();
}

void InstalledPackageListView::taskFailedHandler(const QUuid & _taskId, const QString &message) {
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

void InstalledPackageListView::getInstalledPackages(const QVector<Package *> &packageList){
    boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QWidget *widget=new QWidget;
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
        emptyLabel->setText("Nothing is installed.");
        emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
        boxLayout->addWidget(emptyLabel);
    }
}


