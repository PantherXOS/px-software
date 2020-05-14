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
    auto loading = new QProgressIndicator(this);
    loading->setFixedSize(VIEW_LOADING_ICON_SIZE,VIEW_LOADING_ICON_SIZE);
    loading->startAnimation();

    boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setAlignment(Qt::AlignCenter);
    boxLayout->addWidget(loading);
    auto *widget=new PxQWidget;
    widget->setLayout(boxLayout);
    setWidgetResizable(true);
    setWidget(widget);
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
        auto *widget=new PxQWidget;
        widget->setLayout(boxLayout);
        setWidgetResizable(true);
        setWidget(widget);
    }
}

void InstalledPackageListView::getInstalledPackages(const QVector<Package *> &packageList){
    boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    auto *widget=new PxQWidget;
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
        emptyLabel->setText(tr("Nothing is installed."));
        emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
        boxLayout->addWidget(emptyLabel);
    }
}


