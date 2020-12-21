/*
 * px-software - Qt-GUI package manager for guix on PantherX OS
 * Copyright (C) 2019, 2020  Hamzeh Nasajpour <h.nasajpour@pantherx.org>
 * Copyright (C) 2019, 2020  Reza Alizadeh Majd <r.majd@pantherx.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "MainWindow.h"
#include <sys/statvfs.h>

#include "InstalledPackageListView.h"
#include "UserUpdatablePackageListView.h"
#include "SystemUpdatablePackageListView.h"
#include "InProgressPackageListView.h"

bool getFreeDiskSpace(QString path, QString &result){
    struct statvfs fiData;
    if((statvfs(path.toStdString().c_str(),&fiData)) < 0 ) {
        result = "Failed to stat " + path;
        return false;
    } else {
        auto free_kb = (fiData.f_bsize * fiData.f_bfree)/1024;
        float free_gb;
        if(free_kb > 1024){
            auto free_mb = float(free_kb / 1024);
            if(free_mb > 1024){
                free_gb = float(free_mb / 1024);
                result = QString::number(free_gb, 'f', 1)+"GB";
            } else result = QString::number(free_mb)+"MB";
        } else
            result = QString::number(free_kb)+"KB";
        return true;
    }
}

MainWindow::MainWindow(QString dbPath, QWidget *parent) :
        PXMainWindow("Software", QIcon::fromTheme("panther"), parent){
    CacheManager::init(CACHE_DIR);
    CacheManager::instance()->clear();

    PackageManagerTracker::init(dbPath);
    m_pkgMgrTrkr = PackageManagerTracker::Instance();
    m_pkgMgr = PKG::PackageManager::Instance();
    PXContentWidget *errorView = nullptr;
    if (!m_pkgMgr->isInited()) {
        qDebug() << "Invalid Database Path!";
        errorView = dbErrorHandling();
    } else
        UserUpdateNotification::instance();

    buildSidebar(errorView);
}

void MainWindow::buildSidebar(PXContentWidget *errorView){
    auto storeTitle = new PXSideBarItem("STORE",PXSideBarItem::ItemType::Item, nullptr);
    storeTitle->setFlags(storeTitle->flags() & ~Qt::ItemIsSelectable);
    addItemToSideBar(storeTitle);

    auto latestView = new TagPackageList("Latest", "latest");
    auto latestItem = new PXSideBarItem("Latest", PXSideBarItem::ItemType::Subitem, (errorView?errorView:latestView));
    latestItem->setIcon(QIcon::fromTheme("px-new"));
    addItemToSideBar(latestItem);

    auto recommendedView = new TagPackageList("Recommended", "recommended");
    auto recommendedItem = new PXSideBarItem("Recommended", PXSideBarItem::ItemType::Subitem, (errorView?errorView:recommendedView));
    recommendedItem->setIcon(QIcon::fromTheme("px-recommended"));
    addItemToSideBar(recommendedItem);

    auto categoryView = new CategoryView("Categories");
    auto categoriesItem = new PXSideBarItem("Categories", PXSideBarItem::ItemType::Subitem, (errorView?errorView:categoryView));
    categoriesItem->setIcon(QIcon::fromTheme("px-categories"));
    addItemToSideBar(categoriesItem);

    auto userAppsTitle = new PXSideBarItem("YOUR APPS",PXSideBarItem::ItemType::Item, nullptr);
    userAppsTitle->setFlags(userAppsTitle->flags() & ~Qt::ItemIsSelectable);
    addItemToSideBar(userAppsTitle);

    InstalledPackageListView::init("Installed");
    auto installedView = InstalledPackageListView::Instance();
    installedView->refresh();
    auto installedItem = new PXSideBarItem("Installed", PXSideBarItem::ItemType::Subitem, (errorView?errorView:installedView));
    installedItem->setIcon(QIcon::fromTheme("px-installed"));
    addItemToSideBar(installedItem);

    UserUpdatablePackageListView::init("Updates");
    auto userUpdatesView = UserUpdatablePackageListView::Instance();
    userUpdatesView->refresh();
    userUpdatesItem = new UpdatesItem("Updates", (errorView?errorView:userUpdatesView));
    connect(m_pkgMgrTrkr, 
            SIGNAL(userUpdatablePackageListReady(const QVector<Package *> &)), 
            this, 
            SLOT(getUserUpdatablePackages(const QVector<Package *> &)));
    userUpdatesItem->setIcon(QIcon::fromTheme("px-updates"));
    addItemToSideBar(userUpdatesItem);

    InProgressPackageListView::init("In Progress");
    auto inProgressView = InProgressPackageListView::Instance();
    inProgressItem = new PXSideBarItem("In Progress", PXSideBarItem::ItemType::Subitem, (errorView?errorView:inProgressView));
    connect(m_pkgMgrTrkr, SIGNAL(packageRemoved(const QString &)),this, SLOT(inProgressListUpdated()));
    connect(m_pkgMgrTrkr, SIGNAL(packageInstalled(const QString &)),this, SLOT(inProgressListUpdated()));
    connect(m_pkgMgrTrkr, SIGNAL(packageUpdated(const QString &)),this, SLOT(inProgressListUpdated()));
    connect(m_pkgMgrTrkr, SIGNAL(packageTaskCanceled(const QString &)),this, SLOT(inProgressListUpdated()));
    connect(m_pkgMgrTrkr, SIGNAL(progressFailed(const QString &,const QString&)),this, SLOT(inProgressListUpdated()));
    connect(m_pkgMgrTrkr, SIGNAL(inProgressRequest()),this, SLOT(inProgressListUpdated()));
    inProgressItem->setIcon(QIcon::fromTheme("px-in_progress"));
    addItemToSideBar(inProgressItem);
    inProgressItem->setHidden(true);

    auto sysAppsTitle = new PXSideBarItem("SYSTEM",PXSideBarItem::ItemType::Item, nullptr);
    sysAppsTitle->setFlags(sysAppsTitle->flags() & ~Qt::ItemIsSelectable);
    addItemToSideBar(sysAppsTitle);

    SystemUpdatablePackageListView::init("Updates");
    auto sysUpdatesView = SystemUpdatablePackageListView::Instance();
    sysUpdatesView->refresh();
    sysUpdatesItem = new UpdatesItem("Updates", (errorView?errorView:sysUpdatesView));
    connect(m_pkgMgrTrkr, 
            SIGNAL(systemUpdatablePackageListReady(const QVector<Package *> &)), 
            this, 
            SLOT(getSystemUpdatablePackages(const QVector<Package *> &)));
    sysUpdatesItem->setIcon(QIcon::fromTheme("px-updates"));
    addItemToSideBar(sysUpdatesItem);

    QString diskSpace;
    if(getFreeDiskSpace(QString("/"), diskSpace)){
        diskSpace += QString(" ") + tr("remaining");
    }
    auto bottomItem = new PXSideBarItem(diskSpace ,PXSideBarItem::ItemType::Subitem, nullptr);
    bottomItem->setIcon(QIcon::fromTheme("drive-harddisk"));
    bottomItem->setFlags(bottomItem->flags() & ~Qt::ItemIsEnabled);
    addItemToBottomBar(bottomItem);

    setDefaultItem(latestItem);
}

void MainWindow::inProgressListUpdated(){
    auto pkgs = m_pkgMgrTrkr->inProgressList();
    if(pkgs.size()) {
        inProgressItem->setHidden(false);
    } else {
        inProgressItem->setHidden(true);
    }
}

void MainWindow::getUserUpdatablePackages(const QVector<Package *> &packageList) {
    userUpdatesItem->refreshStatus(packageList.size());
}

void MainWindow::getSystemUpdatablePackages(const QVector<Package *> &packageList) {
    sysUpdatesItem->refreshStatus(packageList.size());
}

MainWindow::~MainWindow() {
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QWidget * const widget = childAt(event->pos());

    auto categoryWidget = qobject_cast<CategoryWidget*>(widget->parentWidget());
    auto packageWidget = qobject_cast<PackageListWidgetItem*>(widget->parentWidget());
    if(widget){
        if(categoryWidget){
            PackageListWidget *packageListWidget = new PackageListWidget(false, categoryWidget->getCategory()->name(),
                                                                         nullptr);
            addContent(packageListWidget);
        } else if(packageWidget){
            connect(packageWidget, SIGNAL(showTerminalSignal(TerminalWidget *)), this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
            auto package = new PackageDetails(packageWidget->getPackage(),
                                                       packageWidget->getPackage()->name(), nullptr);
            connect(package, SIGNAL(showTerminalSignal(TerminalWidget *)), this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
            connect(package, SIGNAL(screenshotItemClicked(ScreenshotItem *)), this, SLOT(screenshotItemClickedHandler(ScreenshotItem *)));
            addContent(package);
        }
    }
}

void MainWindow::searchBoxTextEditedHandler(PXContentWidget *currentWidget, const QString &text){
    SearchPackagesList::SearchFilter filter;
    if(qobject_cast<InstalledPackageListView*>(currentWidget)){
        filter = SearchPackagesList::SearchFilter::Installed;
    } else if (qobject_cast<SearchPackagesList*>(currentWidget)) {
        filter = ((SearchPackagesList *)currentWidget)->currentFilter();
    } else {
        filter = SearchPackagesList::SearchFilter::All;
    }

    auto searchPackageList = new SearchPackagesList(text, filter , nullptr);
    addContent(searchPackageList);
}

void MainWindow::settingsButtonHandler() {
    qDebug() << "TODO - Settings Button Handler";
}

void MainWindow::helpButtonHandler(){
    qDebug() << "TODO - Help Button Handler";
}

void MainWindow::showTerminalSignalHandler(TerminalWidget *terminal){
    addContent(terminal);
}

void MainWindow::screenshotItemClickedHandler(ScreenshotItem *item) {
    ScreenShotViewer *screenShotViewer = new ScreenShotViewer(item);
    addContent(screenShotViewer);
}

PXContentWidget *MainWindow::dbErrorHandling(){
    errorLabel = new QLabel(tr(DB_ERROR_MESSAGE_BEFORE_UPDATE));
    errorLabel->setWordWrap(true);
    auto font = errorLabel->font();
    font.setPointSize(DB_ERROR_MESSAGE_FONT_SIZE);
    errorLabel->setFont(font);
    errorLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    errorLabel->setAlignment(Qt::AlignCenter);

    updateButton = new QPushButton(tr("Update"));
    updateButton->setStyleSheet(PACKAGE_UPDATE_STYLESHEET);
    updateButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
    connect(updateButton, SIGNAL(released()), this, SLOT(updateButtonHandler()));

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(updateButton);
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->setMargin(30);

    auto layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    layout->addWidget(errorLabel);
    layout->addLayout(buttonLayout);
    layout->setMargin(60);
    
    auto _widget = new PXContentWidget("");
    _widget->setLayout(layout);
    return _widget;
}

void MainWindow::updateButtonHandler(){
    errorLabel->setText(DB_ERROR_MESSAGE_AFTER_UPDATE);
    updateButton->setText("Updating ...");
    updateButton->setStyleSheet(PACKAGE_INPROGRESS_STYLESHEET);
    updateButton->setDisabled(true);
    qDebug() << "Running local DB Update ...";
    connect(m_pkgMgr, &PackageManager::dbUpdateError, [=](const QString &result) {
        errorLabel->setText(DB_ERROR_MESSAGE_PULL_IS_IN_BG);
        qWarning() << "Error in running guix pull:" << result;
    });
    m_pkgMgr->requestDBPackageUpdate();
}
