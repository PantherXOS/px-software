//
// Created by hamzeh on 12/31/19.
//

#include "PXMainWindow.h"

PXMainWindow::PXMainWindow(const QString& title, const QIcon& icon,QWidget *parent) : QMainWindow (parent) {
    setWindowTitle(title);
    setWindowIcon(icon);
    //
    sideBar = new PXSideBar;
    connect(sideBar, SIGNAL (itemClicked(QListWidgetItem*)), this, SLOT (sideBarItemHandler(QListWidgetItem*)));

    topBar = new PXTopBar(title, this);
    connect(topBar,SIGNAL(backButtonClicked()),this,SLOT(backButtonPressed()));
    connect(topBar,SIGNAL(forwardButtonClicked()),this,SLOT(forwardButtonPressed()));
    connect(topBar,SIGNAL(settingsButtonClicked()),this,SLOT(settingsButtonPressed()));
    connect(topBar,SIGNAL(helpButtonClicked()),this,SLOT(helpButtonPressed()));
    connect(topBar,SIGNAL(searchKeywordEntered(const QString &)),this,SLOT(searchBoxTextEdited(const QString &)));
    contentWidget = new QStackedWidget;
    contentWidget->showMaximized();
    contentWidget->setStyleSheet(STACKED_WIDGET_STYLE);
    //
    buildWindow();
    auto windownManager = PXWindowManager::instance();
    connect(windownManager,SIGNAL( addPxWindow(const PXContentWidget *)),this,SLOT( addWindowSignalHandler(const PXContentWidget *)));
    //
    setMinimumSize(MAINWINDOW_MIN_SIZE_W, MAINWINDOW_MIN_SIZE_H);
    showMaximized();
}

PXMainWindow::~PXMainWindow(){

}

void PXMainWindow::buildWindow() {
    auto statusBar = new PXStatusBar(this);

    auto sidebarLayout = new QVBoxLayout;
    sidebarLayout->addWidget(sideBar);
    sidebarLayout->addWidget(statusBar);

    auto downLayout = new QHBoxLayout;
    downLayout->addLayout(sidebarLayout);
    downLayout->addWidget(contentWidget);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(topBar);
    mainLayout->addLayout(downLayout);

    window = new QWidget;
    window->setLayout(mainLayout);
    setCentralWidget(window);
}

void PXMainWindow::addItemToSideBar(PXSideBarItem *item) {
    sideBar->addPxItem(item);
}

void PXMainWindow::addListToSideBar(QVector<PXSideBarItem *> list) {
    for(auto item : list)
        sideBar->addPxItem(item);
}

void PXMainWindow::sideBarItemHandler(QListWidgetItem *item) {
    auto listWidgetItem = (PXSideBarItem *) item;
    if(listWidgetItem) {
        auto view = listWidgetItem->getView();
        if(view != nullptr) loadContent(view);
    }
}

void PXMainWindow::loadContent(PXContentWidget *widget) {
    if (widget) {
        int current = contentWidget->currentIndex();
        int max = contentWidget->count() - 1;
        while (current < max) {
            QWidget *_item = contentWidget->widget(max);
//            qDebug() << " delete index: " << max << ", max: " << contentWidget->count() << " = " << _item;
            contentWidget->removeWidget(_item);
            max = contentWidget->count() - 1;
//            delete _item;
        }
        contentWidget->addWidget(widget);
        contentWidget->setCurrentIndex(contentWidget->count() - 1);
        topBar->setAddress(widget->getTitle());
//        qDebug() << " add    index: " << contentWidget->currentIndex() << ", max: " << contentWidget->count() << " = " << + widget;
    }
}

void PXMainWindow::loadContent(PXScrollArea *widget) {
    if (widget) {
        int current = contentWidget->currentIndex();
        int max = contentWidget->count() - 1;
        while (current < max) {
            QWidget *_item = contentWidget->widget(max);
//            qDebug() << " delete index: " << max << ", max: " << contentWidget->count() << " = " << _item;
            contentWidget->removeWidget(_item);
            max = contentWidget->count() - 1;
//            delete _item;
        }
        contentWidget->addWidget(widget);
        contentWidget->setCurrentIndex(contentWidget->count() - 1);
        topBar->setAddress(widget->getTitle());
//        qDebug() << " add    index: " << contentWidget->currentIndex() << ", max: " << contentWidget->count() << " = " << + widget;
    }
}

QWidget *PXMainWindow::currentWidget() {
    return contentWidget->currentWidget();
}

void PXMainWindow::backButtonPressed() {
    int index = contentWidget->currentIndex();
    if(index>0) {
        index--;
        contentWidget->setCurrentIndex(index);
        auto content = qobject_cast<PXContentWidget *>(contentWidget->currentWidget());
        if (content) {
            sideBar->setSelectedItem(content->getTitle());
            topBar->setAddress(content->getTitle());
        }
    }
}

void PXMainWindow::forwardButtonPressed() {
    int index = contentWidget->currentIndex();
    if(index < contentWidget->count()) {
        index++;
        contentWidget->setCurrentIndex(index);
        auto content = qobject_cast<PXContentWidget *>(contentWidget->currentWidget());
        if (content) {
            sideBar->setSelectedItem(content->getTitle());
            topBar->setAddress(content->getTitle());
        }
    }
}

void PXMainWindow::settingsButtonPressed() {
    if(settingsPage!= nullptr)
        loadContent(settingsPage);
}

void PXMainWindow::helpButtonPressed() {
    if(helpPage!= nullptr)
        loadContent(helpPage);
}

void PXMainWindow::searchBoxTextEdited(const QString &text) {
    searchBoxTextEditedHandler(dynamic_cast<PXContentWidget *>(contentWidget->currentWidget()), text);
}

void PXMainWindow::setHelpPage(PXContentWidget *widget) {
    helpPage = widget;
}

void PXMainWindow::setSettingsPage(PXContentWidget *widget) {
    settingsPage = widget;
}

void PXMainWindow::setDefaultView(PXSideBarItem *item) {
    loadContent(item->getView());
}

void PXMainWindow::addWindowSignalHandler(const PXContentWidget *content) {
    loadContent(const_cast<PXContentWidget *>(content));
}
