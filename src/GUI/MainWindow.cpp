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

#include "InstalledPackageListView.h"
#include "UserUpdatablePackageListView.h"
#include "SystemUpdatablePackageListView.h"
#include "InProgressPackageListView.h"

MainWindow::MainWindow(QString dbPath, QWidget *parent) :
        PXMainWindow("Software", QIcon::fromTheme("panther"), parent){
    CacheManager::init(CACHE_DIR);
    CacheManager::instance()->clear();

    PackageManagerTracker::init(dbPath);
    m_pkgMgrTrkr = PackageManagerTracker::Instance();
    m_pkgMgr = PKG::PackageManager::Instance();
    PXScrollArea *errorView = nullptr;
    if (!m_pkgMgr->isInited()) {
        qDebug() << "Invalid Database Path!";
        errorView = dbErrorHandling();
    } else
        UserUpdateNotification::instance();

    buildSidebar(errorView);
}

void MainWindow::buildSidebar(PXScrollArea *errorView){
    auto store = new PXSideBarItem("STORE",PXSideBarItem::ItemType::Item, nullptr);
    store->setFlags(store->flags() & ~Qt::ItemIsSelectable);
    addItemToSideBar(store);

    auto latestView = new TagPackageList("Latest", "latest");
    auto latest = new PXSideBarItem("Latest", PXSideBarItem::ItemType::Subitem, (errorView?errorView:latestView));
    latest->setIcon(QIcon::fromTheme("px-new"));
    addItemToSideBar(latest);

    auto recommendedView = new TagPackageList("Recommended", "recommended");
    auto recommended = new PXSideBarItem("Recommended", PXSideBarItem::ItemType::Subitem, (errorView?errorView:recommendedView));
    recommended->setIcon(QIcon::fromTheme("px-recommended"));
    addItemToSideBar(recommended);

    auto categoryView = new CategoryView("Categories");
    auto categories = new PXSideBarItem("Categories", PXSideBarItem::ItemType::Subitem, (errorView?errorView:categoryView));
    categories->setIcon(QIcon::fromTheme("px-categories"));
    addItemToSideBar(categories);

    auto userApps = new PXSideBarItem("YOUR APPS",PXSideBarItem::ItemType::Item, nullptr);
    userApps->setFlags(store->flags() & ~Qt::ItemIsSelectable);
    addItemToSideBar(userApps);

    InstalledPackageListView::init("Installed");
    auto installedView = InstalledPackageListView::Instance();
    installedView->refresh();
    auto installed = new PXSideBarItem("Installed", PXSideBarItem::ItemType::Subitem, (errorView?errorView:installedView));
    installed->setIcon(QIcon::fromTheme("px-installed"));
    addItemToSideBar(installed);

    UserUpdatablePackageListView::init("Updates");
    auto userUpdatesView = UserUpdatablePackageListView::Instance();
    userUpdatesView->refresh();
    userUpdates = new UpdatesItem("Updates", (errorView?errorView:userUpdatesView));
    connect(m_pkgMgrTrkr, 
            SIGNAL(userUpdatablePackageListReady(const QVector<Package *> &)), 
            this, 
            SLOT(getUserUpdatablePackages(const QVector<Package *> &)));
    userUpdates->setIcon(QIcon::fromTheme("px-updates"));
    addItemToSideBar(userUpdates);

    InProgressPackageListView::init("In Progress");
    auto inProgressView = InProgressPackageListView::Instance();
    inProgressView->refresh();
    auto inProgress = new PXSideBarItem("In Progress", PXSideBarItem::ItemType::Subitem, (errorView?errorView:inProgressView));
    inProgress->setIcon(QIcon::fromTheme("px-in_progress"));
    addItemToSideBar(inProgress);

    auto sysApps = new PXSideBarItem("SYSTEM",PXSideBarItem::ItemType::Item, nullptr);
    sysApps->setFlags(store->flags() & ~Qt::ItemIsSelectable);
    addItemToSideBar(sysApps);

    SystemUpdatablePackageListView::init("Updates");
    auto sysUpdatesView = SystemUpdatablePackageListView::Instance();
    sysUpdatesView->refresh();
    sysUpdates = new UpdatesItem("Updates", (errorView?errorView:sysUpdatesView));
    connect(m_pkgMgrTrkr, 
            SIGNAL(systemUpdatablePackageListReady(const QVector<Package *> &)), 
            this, 
            SLOT(getSystemUpdatablePackages(const QVector<Package *> &)));
    sysUpdates->setIcon(QIcon::fromTheme("px-updates"));
    addItemToSideBar(sysUpdates);

    setDefaultView(latest);
}

void MainWindow::getUserUpdatablePackages(const QVector<Package *> &packageList) {
    userUpdates->refreshStatus(packageList.size());
}

void MainWindow::getSystemUpdatablePackages(const QVector<Package *> &packageList) {
    sysUpdates->refreshStatus(packageList.size());
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
            loadContent(packageListWidget);
        } else if(packageWidget){
            connect(packageWidget, SIGNAL(showTerminalSignal(TerminalWidget *)), this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
            auto package = new PackageDetails(packageWidget->getPackage(),
                                                       packageWidget->getPackage()->name(), nullptr);
            connect(package, SIGNAL(showTerminalSignal(TerminalWidget *)), this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
            connect(package, SIGNAL(screenshotItemClicked(ScreenshotItem *)), this, SLOT(screenshotItemClickedHandler(ScreenshotItem *)));
            loadContent(package);
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
    loadContent(searchPackageList);
}

// void MainWindow::showTerminalSignalHandler(TerminalWidget *terminal){
//     refreshContentLayouts(terminal);
// }

// void MainWindow::screenshotItemClickedHandler(ScreenshotItem *item) {
//     ScreenShotViewer *screenShotViewer = new ScreenShotViewer(item);
//     refreshContentLayouts(screenShotViewer);
// }

// void MainWindow::refreshContentLayouts(QWidget *item) {
//     if (item) {
//         int current = contentLayouts->currentIndex();
//         int max = contentLayouts->count() - 1;
//         while (current < max) {
//             QWidget *_item = contentLayouts->widget(max);
// //        qDebug() << " delete index: " << max << ", max: " << contentLayouts->count() << " = " << _item;
//             contentLayouts->removeWidget(_item);
//             max = contentLayouts->count() - 1;
//             if (!qobject_cast<InProgressPackageListView *>(_item) &&
//                 !qobject_cast<InstalledPackageListView *>(_item) &&
//                 !qobject_cast<TerminalWidget *>(_item) &&
//                 !qobject_cast<UserUpdatablePackageListView *>(_item) &&
//                 !qobject_cast<SystemUpdatablePackageListView *>(_item)) {
//                 delete _item; // TODO Should be check for old view deletion
//             }
//         }
// //        item->setStyleSheet(CONTENT_WIDGET_STYLE);
//         contentLayouts->addWidget(item);
//         contentLayouts->setCurrentIndex(contentLayouts->count() - 1);
//         reloadTopBar();
// //    qDebug() << " add    index: " << contentLayouts->currentIndex() << ", max: " << contentLayouts->count() << " = " << + item;
//     }
// }

PXScrollArea *MainWindow::dbErrorHandling(){
    auto pal = QGuiApplication::palette();
    auto bgColor = pal.color(QPalette::Active, QPalette::Base);
    auto fgColor = pal.color(QPalette::Active, QPalette::Text);

    errorLabel = new QLabel(tr(DB_ERROR_MESSAGE_BEFORE_UPDATE));
    errorLabel->setWordWrap(true);
    auto font = errorLabel->font();
    font.setPointSize(DB_ERROR_MESSAGE_FONT_SIZE);
    errorLabel->setFont(font);
    errorLabel->setStyleSheet(QString(QLABEL_STYLE_FROM_COLOR_SCHEME).arg(bgColor.name(), fgColor.name()));
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
    auto widget = new PXScrollArea("");
    widget->setLayout(layout);
    return widget;
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