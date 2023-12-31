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
#include <QDesktopServices>

#include "InstalledPackageListView.h"
#include "UserUpdatablePackageListView.h"
#include "SystemUpdatablePackageListView.h"
#include "InProgressPackageListView.h"
#include "CategoryPackageListView.h"
#include <QUrl>
#include <QEventLoop>
#include <QStorageInfo>
#include <PXProgressIndicator.h>
#include "DownloadManager.h"

bool getFreeDiskSpace(QString path, QString &result){
    QStorageInfo storage = QStorageInfo::root();
    auto free_kb = storage.bytesAvailable()/1024;
    qDebug() << "HDD info: " << storage.rootPath() << storage.name() << storage.fileSystemType() << free_kb << "KB";

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

MainWindow::MainWindow(const QMap<QString, QString> &urlArgs, const QString &dbPath, QWidget *parent) :
        PXMainWindow("Software", QIcon::fromTheme("panther"), "px-desktop-wiki:page=Software/index.html", parent){
    searchBox()->addAction(QIcon::fromTheme("system-search"), "SEARCH ...", QLineEdit::TrailingPosition);
    DownloadManager::Instance();
    CacheManager::init(CACHE_DIR);
    CacheManager::instance()->clear();
    PackageManagerTracker::init(dbPath);
    m_pkgMgrTrkr = PackageManagerTracker::Instance();
    m_pkgMgr = PKG::PackageManager::Instance();

    UserUpdateNotification::instance();
    _dbUpdatingView = dbUpdatingView();
    addContent(_dbUpdatingView);

    connect(m_pkgMgr, &PackageManager::dbUpdated, [&](bool result){
        m_pkgMgr->reload();
        if(!m_pkgMgr->isInited()){
            qDebug() << "Software DB has not been initated yet!";
            addContent(dbUpdatingErrorView());
        } else {
            if(result) 
                qDebug() << "Software DB updated successfully!";
            auto list = urlArgs[APPLIST_ARG_TILTE];
            auto apps = urlArgs[APP_ARG_TITLE];
            buildSidebar(list);
            setSideBarVisible(true);
            if(!apps.isEmpty()) {
                searchBox()->setText(apps);
                emit searchBox()->lineEdit()->returnPressed();
            }
            if(_dbUpdatingView)
                removeContent(_dbUpdatingView);
        }
    });
    
    m_pkgMgr->checkDBupdate();    
    setSideBarVisible(false);
}

void MainWindow::buildSidebar(const QString &list){
    auto storeTitle = new PXSideBarItem("STORE",PXSideBarItem::ItemType::Item, nullptr);
    storeTitle->setFlags(storeTitle->flags() & ~Qt::ItemIsSelectable);
    addItemToSideBar(storeTitle);

    auto latestView = new TagPackageList(LATEST_APPS_TITLE, LATEST_APPS_TAG);
    connect(latestView, &TagPackageList::packageItemClicked, [&](PKG::Package *pkg){
        auto package = new PackageDetails(pkg, pkg->name(), nullptr);
        addContent(package);
    });
    connect(latestView, SIGNAL(terminalWidgetClicked(TerminalWidget *)), this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
    auto latestItem = new PXSideBarItem(LATEST_APPS_TITLE, PXSideBarItem::ItemType::Subitem, latestView);
    latestItem->setIcon(QIcon::fromTheme("px-new"));
    addItemToSideBar(latestItem);

    auto recommendedView = new TagPackageList(RECOMENDDED_APPS_TITLE, RECOMENDDED_APPS_TAG);
    connect(recommendedView, &TagPackageList::packageItemClicked, [&](PKG::Package *pkg){
        auto package = new PackageDetails(pkg, pkg->name(), nullptr);
        addContent(package);
    });
    connect(recommendedView, SIGNAL(terminalWidgetClicked(TerminalWidget *)), this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
    auto recommendedItem = new PXSideBarItem(RECOMENDDED_APPS_TITLE, PXSideBarItem::ItemType::Subitem, recommendedView);
    recommendedItem->setIcon(QIcon::fromTheme("px-recommended"));
    addItemToSideBar(recommendedItem);

    auto categoryView = new CategoryView(CATEGORIES_ITEM_TITLE);
    connect(categoryView,&CategoryView::categoryItemClicked,[&](const QString & category){
        CategoryPackageListView *categoryPackageListView = new CategoryPackageListView(false, category, nullptr);
        connect(categoryPackageListView, &CategoryPackageListView::packageItemClicked, [&](PKG::Package *pkg){
            auto package = new PackageDetails(pkg, pkg->name(), nullptr);
            addContent(package);
        });
        connect(categoryPackageListView, SIGNAL(terminalWidgetClicked(TerminalWidget *)), this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
        addContent(categoryPackageListView);
    });
    auto categoriesItem = new PXSideBarItem(CATEGORIES_ITEM_TITLE, PXSideBarItem::ItemType::Subitem, categoryView);
    categoriesItem->setIcon(QIcon::fromTheme("px-categories"));
    addItemToSideBar(categoriesItem);

    auto userAppsTitle = new PXSideBarItem("YOUR APPS",PXSideBarItem::ItemType::Item, nullptr);
    userAppsTitle->setFlags(userAppsTitle->flags() & ~Qt::ItemIsSelectable);
    addItemToSideBar(userAppsTitle);

    InstalledPackageListView::init(INSTALLED_APPS_TITLE);
    auto installedView = InstalledPackageListView::Instance();
    connect(installedView, &TagPackageList::packageItemClicked, [&](PKG::Package *pkg){
        auto package = new PackageDetails(pkg, pkg->name(), nullptr);
        addContent(package);
    });
    connect(installedView, SIGNAL(terminalWidgetClicked(TerminalWidget *)), this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
    installedView->refresh();
    auto installedItem = new PXSideBarItem(INSTALLED_APPS_TITLE, PXSideBarItem::ItemType::Subitem, installedView);
    installedItem->setIcon(QIcon::fromTheme("px-installed"));
    addItemToSideBar(installedItem);

    UserUpdatablePackageListView::init(USER_UPDATES_TITLE);
    userUpdatesView = UserUpdatablePackageListView::Instance();
    connect(userUpdatesView, &TagPackageList::packageItemClicked, [&](PKG::Package *pkg){
        auto package = new PackageDetails(pkg, pkg->name(), nullptr);
        addContent(package);
    });
    connect(userUpdatesView, SIGNAL(terminalWidgetClicked(TerminalWidget *)), this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
    userUpdatesView->refresh();
    userUpdatesItem = new UpdatesItem(USER_UPDATES_TITLE, userUpdatesView);
    connect(m_pkgMgrTrkr, 
            SIGNAL(userUpdatablePackageListReady(const QVector<Package *> &)), 
            this, 
            SLOT(getUserUpdatablePackages(const QVector<Package *> &)));
    userUpdatesItem->setIcon(QIcon::fromTheme("px-updates"));
    addItemToSideBar(userUpdatesItem);

    InProgressPackageListView::init(IN_PROGRESS_APPS_TITLE);
    auto inProgressView = InProgressPackageListView::Instance();
    connect(inProgressView, &TagPackageList::packageItemClicked, [&](PKG::Package *pkg){
        auto package = new PackageDetails(pkg, pkg->name(), nullptr);
        addContent(package);
    });
    connect(inProgressView, SIGNAL(terminalWidgetClicked(TerminalWidget *)), this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
    inProgressItem = new InProgressItem(IN_PROGRESS_APPS_TITLE, inProgressView);
    connect(m_pkgMgrTrkr, &PackageManagerTracker::packageRemoved,[&](const QString &){
        inProgressListUpdated(PACKAGE_REMOVED);
    });
    connect(m_pkgMgrTrkr, &PackageManagerTracker::packageInstalled,[&](const QString &){
        inProgressListUpdated(PACKAGE_INSTALLED);
    });
    connect(m_pkgMgrTrkr, &PackageManagerTracker::packageUpdated,[&](const QString &){
        inProgressListUpdated(PACKAGE_UPDATED);
    });
    connect(m_pkgMgrTrkr, &PackageManagerTracker::packageTaskCanceled,[&](const QString &){
        inProgressListUpdated(PACKAGE_ACTION_CANCELED);
    });
    connect(m_pkgMgrTrkr, &PackageManagerTracker::progressFailed,[&](const QString &,const QString&){
        inProgressListUpdated(PACKAGE_ACTION_FAILED);
    });
    connect(m_pkgMgrTrkr, &PackageManagerTracker::inProgressRequest,[&](){
        inProgressListUpdated(ANY);
    });

    inProgressItem->setIcon(QIcon::fromTheme("px-in_progress"));
    addItemToSideBar(inProgressItem);
    inProgressItem->setHidden(true);

    auto sysAppsTitle = new PXSideBarItem("SYSTEM",PXSideBarItem::ItemType::Item, nullptr);
    sysAppsTitle->setFlags(sysAppsTitle->flags() & ~Qt::ItemIsSelectable);
    addItemToSideBar(sysAppsTitle);

    SystemUpdatablePackageListView::init(SYSTEM_UPDATES_TITLE);
    auto sysUpdatesView = SystemUpdatablePackageListView::Instance();
    connect(sysUpdatesView, &TagPackageList::packageItemClicked, [&](PKG::Package *pkg){
        auto package = new PackageDetails(pkg, pkg->name(), nullptr);
        addContent(package);
    });
    connect(sysUpdatesView, SIGNAL(terminalWidgetClicked(TerminalWidget *)), this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
    sysUpdatesView->refresh();
    sysUpdatesItem = new UpdatesItem(SYSTEM_UPDATES_TITLE, sysUpdatesView);
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

    if(list==RECOMENDDED_APPS_TAG)
        setDefaultItem(recommendedItem);
    else if(list==CATEGORIES_ITEM_TAG)
        setDefaultItem(categoriesItem);
    else if(list==INSTALLED_APPS_TAG)
        setDefaultItem(installedItem);
    else if(list==USER_UPDATES_TAG)
        setDefaultItem(userUpdatesItem);
    else if(list==IN_PROGRESS_APPS_TAG)
        setDefaultItem(inProgressItem);
    else if(list==SYSTEM_UPDATES_TAG)
        setDefaultItem(sysUpdatesItem);
    else
        setDefaultItem(latestItem);
}

void MainWindow::inProgressListUpdated(PACKAGE_ACTIONS action){
    if(action == PACKAGE_REMOVED || action == PACKAGE_UPDATED){
        userUpdatesView->refresh();
    }
    auto pkgs = m_pkgMgrTrkr->inProgressList();
    if(pkgs.size()) {
        inProgressItem->setHidden(false);
        inProgressItem->refreshStatus(pkgs.size());
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

void MainWindow::closeEvent(QCloseEvent *event){
    if(screenShotViewer){
        screenShotViewer->close();
        delete screenShotViewer;
    }
    event->accept();
}

void MainWindow::addContent (PXContentWidget *widget){
    auto packageWidget = qobject_cast<PackageDetails*>(widget);
    if(packageWidget){
        connect(packageWidget, SIGNAL(showTerminalSignal(TerminalWidget *)), this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
        connect(packageWidget, SIGNAL(screenshotItemClicked(ScreenshotItem *)), this, SLOT(screenshotItemClickedHandler(ScreenshotItem *)));
    }
    PXMainWindow::addContent(widget);
}

void MainWindow::searchBoxTextEdited(const QString &text){
    auto currentWidget = contentWidget()->currentWidget();
    SearchPackagesList::SearchFilter filter;
    if(qobject_cast<InstalledPackageListView*>(currentWidget)){
        filter = SearchPackagesList::SearchFilter::Installed;
    } else if(qobject_cast<SystemUpdatablePackageListView*>(currentWidget) || qobject_cast<UserUpdatablePackageListView*>(currentWidget)){
        filter = SearchPackagesList::SearchFilter::Upgradable;
    } else if (qobject_cast<SearchPackagesList*>(currentWidget)) {
        filter = ((SearchPackagesList *)currentWidget)->currentFilter();
    } else {
        filter = SearchPackagesList::SearchFilter::All;
    }

    auto searchPackageList = new SearchPackagesList(text, filter , nullptr);
    connect(searchPackageList, &SearchPackagesList::packageItemClicked, [&](PKG::Package *pkg){
        auto package = new PackageDetails(pkg, pkg->name(), nullptr);
        addContent(package);
    });
    connect(searchPackageList, SIGNAL(terminalWidgetClicked(TerminalWidget *)), this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
    addContent(searchPackageList);
}

void MainWindow::settingsButtonPressed() {
    QDesktopServices::openUrl(QUrl("px-settings-software:"));
}

void MainWindow::showTerminalSignalHandler(TerminalWidget *terminal){
    addContent(terminal);
}

void MainWindow::screenshotItemClickedHandler(ScreenshotItem *item) {
    if(screenShotViewer) {
        screenShotViewer->close();
        delete screenShotViewer;
    }
    screenShotViewer = new ScreenShotViewer(item);
    screenShotViewer->showMaximized();
}

PXContentWidget *MainWindow::dbUpdatingErrorView(){
    auto errorLabel = new QLabel(tr(UPDATE_DB_MESSAGE_UPDATE_ERROR));
    errorLabel->setWordWrap(true);
    auto font = errorLabel->font();
    font.setPointSize(UPDATE_DB_MESSAGE_FONT_SIZE);
    errorLabel->setFont(font);
    errorLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setContentsMargins(0,20,0,50);

    auto layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    layout->addWidget(errorLabel);
    layout->setMargin(60);
    
    auto _widget = new PXContentWidget("");
    _widget->setLayout(layout);
    return _widget;
}

PXContentWidget *MainWindow::dbUpdatingView(){
    auto errorLabel = new QLabel(tr(UPDATE_DB_MESSAGE_BEFORE_UPDATE));
    errorLabel->setWordWrap(true);
    auto font = errorLabel->font();
    font.setPointSize(UPDATE_DB_MESSAGE_FONT_SIZE);
    errorLabel->setFont(font);
    errorLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setContentsMargins(0,20,0,50);

    auto pi = new PXProgressIndicator(this);
    pi->setFixedSize(VIEW_LOADING_ICON_SIZE,VIEW_LOADING_ICON_SIZE);
    pi->startAnimation();
    auto piLayout = new QHBoxLayout();
    piLayout->addWidget(pi);
    piLayout->setAlignment(Qt::AlignCenter);

    auto layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    layout->addWidget(errorLabel);
    layout->addLayout(piLayout);
    layout->setMargin(60);
    
    auto _widget = new PXContentWidget("");
    _widget->setLayout(layout);
    return _widget;
}
