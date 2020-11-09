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

MainWindow::MainWindow(QString dbPath, QWidget *parent) :
        QMainWindow(parent){
    qDebug() << "Database loaded from: " << dbPath;

    CacheManager::init(CACHE_DIR);
    CacheManager::instance()->clear();

    PackageManagerTracker::init(dbPath);
    setMinimumSize(MAINWINDOW_MIN_SIZE_W, MAINWINDOW_MIN_SIZE_H);
    showMaximized();
    setWindowIcon(QIcon::fromTheme("panther"));
    setWindowTitle("Software");
    UserUpdateNotification::instance();

    if (dbPath.isEmpty()) {
        qDebug() << "Invalid Database Path!";
        loadWindow(CONTENT_SECTIONS::ERROR_PAGE);
    } else {
        loadWindow(CONTENT_SECTIONS::STORE_LATEST);
    }

}

MainWindow::~MainWindow() {
}
// --------------------------------------------------------------------------- signal-slot handlers
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QWidget * const widget = childAt(event->pos());

    auto categoryWidget = qobject_cast<CategoryWidget*>(widget->parentWidget());
    auto packageWidget = qobject_cast<PackageListWidgetItem*>(widget->parentWidget());
    if(widget){
//        if(!categoryWidget)
//            categoryWidget = qobject_cast<CategoryWidget*>(widget->parentWidget());
        if(categoryWidget){
            PackageListWidget *packageListWidget = new PackageListWidget(false, categoryWidget->getCategory()->name(),
                                                                         nullptr);
            refreshContentLayouts(packageListWidget);
        } else if(packageWidget){
            connect(packageWidget, SIGNAL(showTerminalSignal(TerminalWidget *)), this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
            QScrollArea * package = new PackageDetails(packageWidget->getPackage(),
                                                       packageWidget->getPackage()->name(), nullptr);
            connect(package, SIGNAL(showTerminalSignal(TerminalWidget *)), this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
            connect(package, SIGNAL(screenshotItemClicked(ScreenshotItem *)), this, SLOT(screenshotItemClickedHandler(ScreenshotItem *)));
            refreshContentLayouts(package);
        }
    }
}

void MainWindow::showTerminalSignalHandler(TerminalWidget *terminal){
    refreshContentLayouts(terminal);
}

void MainWindow::screenshotItemClickedHandler(ScreenshotItem *item) {
    ScreenShotViewer *screenShotViewer = new ScreenShotViewer(item);
    refreshContentLayouts(screenShotViewer);
}

void MainWindow::settingsButtonHandler() {
    cout << "TBD - settingsButtonHandler" << endl;
}

void MainWindow::backButtonHandler() {
    int index = contentLayouts->currentIndex();
    if(index) {
        index--;
        contentLayouts->setCurrentIndex(index);
        reloadTopBar();
    }
}

void MainWindow::forwardButtonHandler() {
    int index = contentLayouts->currentIndex();
    if(index < contentLayouts->count()) {
        index++;
        contentLayouts->setCurrentIndex(index);
        reloadTopBar();
    }
}

void MainWindow::helpButtonHandler() {
    cout << "TBD - helpButtonHandler" << endl;
}

void MainWindow::refreshContentLayouts(QWidget *item) {
    if (item) {
        int current = contentLayouts->currentIndex();
        int max = contentLayouts->count() - 1;
        while (current < max) {
            QWidget *_item = contentLayouts->widget(max);
//        qDebug() << " delete index: " << max << ", max: " << contentLayouts->count() << " = " << _item;
            contentLayouts->removeWidget(_item);
            max = contentLayouts->count() - 1;
            if (!qobject_cast<InProgressPackageListView *>(_item) &&
                !qobject_cast<InstalledPackageListView *>(_item) &&
                !qobject_cast<TerminalWidget *>(_item) &&
                !qobject_cast<UserUpdatablePackageListView *>(_item) &&
                !qobject_cast<SystemUpdatablePackageListView *>(_item)) {
                delete _item; // TODO Should be check for old view deletion
            }
        }
//        item->setStyleSheet(CONTENT_WIDGET_STYLE);
        contentLayouts->addWidget(item);
        contentLayouts->setCurrentIndex(contentLayouts->count() - 1);
        reloadTopBar();
//    qDebug() << " add    index: " << contentLayouts->currentIndex() << ", max: " << contentLayouts->count() << " = " << + item;
    }
}

void MainWindow::leftPanelItemHandler(QListWidgetItem *item) {
    auto listWidgetItem = (PxQListWidgetItem *) item;
    if(listWidgetItem){
        auto view = listWidgetItem->getView();
        refreshContentLayouts(view);
    }
}

void MainWindow::searchBoxHandler(const QString &text){
    auto currentWidget = contentLayouts->currentWidget();

    SearchPackagesList::SearchFilter filter;
    if(qobject_cast<InstalledPackageListView*>(currentWidget)){
        filter = SearchPackagesList::SearchFilter::Installed;
    } else if (qobject_cast<SearchPackagesList*>(currentWidget)) {
        filter = ((SearchPackagesList *)currentWidget)->currentFilter();
    } else {
        filter = SearchPackagesList::SearchFilter::All;
    }

    auto searchPackageList = new SearchPackagesList(text, filter , nullptr);
    refreshContentLayouts(searchPackageList);
}

// -------------------------------------------------------------------------------- ui form objects
QToolBar *MainWindow::loadTopMenu() {
    settingsButton = new QPushButton(this);
    backButton = new QPushButton(this);
    forwardButton = new QPushButton(this);
    helpButton = new QPushButton(this);
    addressBar =new PxSearchBar(this);

    const QSize buttonSize = QSize(TOP_MENU_BUTTON_SIZE, TOP_MENU_BUTTON_SIZE);
    const QSize iconSize = QSize(TOP_MENU_ICON_SIZE, TOP_MENU_ICON_SIZE);
    settingsButton->setFixedSize(buttonSize);
    backButton->setFixedSize(buttonSize);
    forwardButton->setFixedSize(buttonSize);
    helpButton->setFixedSize(buttonSize);
    settingsButton->setIcon(QIcon::fromTheme("px-settings"));
    settingsButton->setIconSize(iconSize);

    backButton->setIcon(QIcon::fromTheme("go-previous"));
    backButton->setIconSize(iconSize);

    forwardButton->setIcon(QIcon::fromTheme("go-next"));
    forwardButton->setIconSize(iconSize);

    helpButton->setIcon(QIcon::fromTheme("px-help"));
    helpButton->setIconSize(iconSize);
    addressBar->setAddress("Software/", "");

    /// todo completer
    /// Connect the "released" signal of buttons to it's slots (signal handler)
    connect(settingsButton, SIGNAL(released()), this, SLOT(settingsButtonHandler()));
    connect(backButton, SIGNAL (released()), this, SLOT (backButtonHandler()));
    connect(forwardButton, SIGNAL (released()), this, SLOT (forwardButtonHandler()));
    connect(helpButton, SIGNAL (released()), this, SLOT (helpButtonHandler()));
    connect(addressBar, SIGNAL(newUserInputReceived(const QString&)), this, SLOT(searchBoxHandler(const QString &)));

    auto toolbar = new QToolBar(this);

    toolbar->addWidget(settingsButton);
    toolbar->addWidget(backButton);
    toolbar->addWidget(forwardButton);
    toolbar->addWidget(addressBar);
    toolbar->addWidget(helpButton);
    return toolbar;
}
// ------------------------------------------------------------------------------ reload ui objects
void MainWindow::loadWindow(int id) {
    contentList = new ContentList();

    auto sidebarLayout = new QVBoxLayout;
    sidebarLayout->addWidget(contentList);
    sidebarLayout->addWidget(createBottombar());

    contentLayouts = new QStackedWidget;
    contentLayouts->showMaximized();

    QHBoxLayout *downLayout = new QHBoxLayout;
    downLayout->addLayout(sidebarLayout);
    downLayout->addWidget(contentLayouts);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(loadTopMenu());
    mainLayout->addLayout(downLayout);

    window = new QWidget;
    window->setLayout(mainLayout);
    setCentralWidget(window);

    if(id==CONTENT_SECTIONS::ERROR_PAGE){
        refreshContentLayouts(dbErrorHandling());
    } else {
        connect(contentList, SIGNAL (itemClicked(QListWidgetItem*)), this, SLOT (leftPanelItemHandler(QListWidgetItem*)));
        refreshContentLayouts(contentList->getItem(id));
    }
}

bool MainWindow::getFreeDiskSpace(QString path, QString &result){
    struct statvfs fiData;
    if((statvfs(path.toStdString().c_str(),&fiData)) < 0 ) {
        result = "Failed to stat " + path;
        return false;
    } else {
//        printf("Disk %s: \n", "/");
//        printf("\tblock size: %u\n", fiData.f_bsize);
//        printf("\ttotal no blocks: %i\n", fiData.f_blocks);
//        printf("\tfree blocks: %i\n", fiData.f_bfree);
        auto free_kb = (fiData.f_bsize * fiData.f_bfree)/1024;
        if(free_kb > 1024){
            auto free_mb = float(free_kb / 1024);
            if(free_mb > 1024){
                auto free_gb = float(free_mb / 1024);
                result = QString::number(free_gb, 'g', 2)+"GB";
            } else result = QString::number(free_mb)+"MB";
        } else
            result = QString::number(free_kb)+"KB";
        return true;
    }
}

QWidget *MainWindow::createBottombar() {
    QFont bottomFont("default", BOTTOMBAR_FONT_SIZE,QFont::Normal);
    QSize size(BOTTOMBAR_ICON_SIZE,BOTTOMBAR_ICON_SIZE);

    QString diskSpace;
    if(getFreeDiskSpace(QString("/"), diskSpace)){
        diskSpace += QString(" ") + tr("remaining");
    }
    auto statusbar = new QLabel(this);
    statusbar->setText(diskSpace);
    statusbar->setFont(bottomFont);

    QIcon _icon(QIcon::fromTheme("drive-harddisk"));
    QPixmap pixmap = _icon.pixmap(size, QIcon::Normal, QIcon::On);

    auto iconLabel = new QLabel(this);
    iconLabel->setPixmap(pixmap);
    iconLabel->setFixedSize(size);

    auto layout = new QHBoxLayout();
    layout->addWidget(iconLabel);
    layout->addWidget(statusbar);

    auto widget = new QWidget(this);
    widget->setLayout(layout);
    return widget;
}

void MainWindow::reloadTopBar(){
    auto categoryWidget = qobject_cast<CategoryWidget*>(contentLayouts->currentWidget());
    auto packageWidget = qobject_cast<PackageListWidgetItem*>(contentLayouts->currentWidget());
    auto packageDetailsWidget = qobject_cast<PackageDetails*>(contentLayouts->currentWidget());
    auto searchPackageWidget = qobject_cast<SearchPackagesList*>(contentLayouts->currentWidget());
    auto screenshotWidget = qobject_cast<ScreenShotViewer*>(contentLayouts->currentWidget());

    if(categoryWidget){
        packageName = "";
        viewName=((CategoryWidget*)categoryWidget)->getCategory()->name();
    }
    else if(packageWidget) {
        packageName = ((PackageListWidgetItem *) packageWidget)->getPackage()->title();
    }
    else if(packageDetailsWidget) {
        packageName = ((PackageDetails *) packageDetailsWidget)->getTitle();
    } else if(searchPackageWidget) {
        packageName = ((SearchPackagesList *) searchPackageWidget)->getTitle();
    } else if(screenshotWidget){
        packageName = ((ScreenShotViewer *) screenshotWidget)->getTitle();
    } else {
        packageName = "";
        viewName = ((PxQScrollArea *)(contentLayouts->currentWidget()))->getTitle();
    }
    contentList->setSelectedItem(viewName);
    addressBar->setAddress(QString("Software/") + viewName + QString("/") , packageName);
    if(contentLayouts->count()==1) {
        backButton->setDisabled(true);
        forwardButton->setDisabled(true);
    } else {
        backButton->setDisabled(false);
        forwardButton->setDisabled(false);
    }
}

PxQScrollArea *MainWindow::dbErrorHandling(){
    auto errorLabel = new QLabel(tr(DB_ERROR_MESSAGE));
    errorLabel->setWordWrap(true);
    auto font = errorLabel->font();
    font.setPointSize(DB_ERROR_MESSAGE_FONT_SIZE);
    errorLabel->setFont(font);

    auto layout = new QHBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(errorLabel);
    auto widget = new PxQScrollArea("");
    widget->setLayout(layout);
    return widget;
}