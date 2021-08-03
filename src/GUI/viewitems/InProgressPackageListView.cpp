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

#include "InProgressPackageListView.h"

InProgressPackageListView *InProgressPackageListView::_instance = nullptr;

InProgressPackageListView *InProgressPackageListView::Instance() {
    if(_instance==nullptr){
        qCritical() << "InProgressPackageListView is not initiated";
    }
    return _instance;
}

void InProgressPackageListView::init(const QString &title) {
    if(_instance==nullptr)
        _instance = new InProgressPackageListView(title, nullptr);
}

void InProgressPackageListView::packageProgressDoneHandler(const QString &packageName) {
    refresh();
}

void InProgressPackageListView::packageProgressDoneHandler(const QString &packageName,const QString &message) {
    refresh();
}

InProgressPackageListView::InProgressPackageListView(const QString &title, PXContentWidget *parent)
        : PackageListWidget(title, parent) {
    connect(m_pkgMgrTrkr, SIGNAL(packageRemoved(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrkr, SIGNAL(packageInstalled(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrkr, SIGNAL(packageUpdated(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrkr, SIGNAL(packageTaskCanceled(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrkr, SIGNAL(progressFailed(const QString &,const QString&)),this, SLOT(packageProgressDoneHandler(const QString &, const QString&)));
    connect(m_pkgMgrTrkr, SIGNAL(inProgressRequest()),this, SLOT(refresh()));
    setWidgetResizable(true);
}

void InProgressPackageListView::refresh() {
    setLoadingVisible(false);
    setListVisible(true);
    clearList();
    QVector<Package *> pkgs = m_pkgMgrTrkr->inProgressList();
    setLoadingVisible(false);
    setListVisible(true);
    if(pkgs.size()) {
        for(auto pkg:pkgs) {
            auto packageWidget = new PackageListWidgetItem(pkg, true, false, this);
            addItem(packageWidget);
        }
    } else {
        auto emptyLabel = new QLabel;
        emptyLabel->setText(tr("All is done"));
        emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
        addItem(emptyLabel);
    }
}
