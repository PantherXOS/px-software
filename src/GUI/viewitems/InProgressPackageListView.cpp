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

InProgressPackageListView::InProgressPackageListView(const QString &title, PXScrollArea *parent)
        : PXScrollArea(title, parent) {
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    connect(m_pkgMgrTrk, SIGNAL(packageRemoved(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrk, SIGNAL(packageInstalled(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrk, SIGNAL(packageUpdated(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrk, SIGNAL(packageTaskCanceled(const QString &)),this, SLOT(packageProgressDoneHandler(const QString &)));
    connect(m_pkgMgrTrk, SIGNAL(progressFailed(const QString &,const QString&)),this, SLOT(packageProgressDoneHandler(const QString &, const QString&)));
    connect(m_pkgMgrTrk, SIGNAL(inProgressRequest()),this, SLOT(refresh()));
}

void InProgressPackageListView::refresh() {
    QVector<Package *> pkgs = m_pkgMgrTrk->inProgressList();
    boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    auto *widget=new PXWidget;
    widget->setLayout(boxLayout);
    setWidgetResizable(true);
    setWidget(widget);
    if(pkgs.size()) {
        for(auto pkg:pkgs) {
            auto packageWidget = new PackageListWidgetItem(pkg, false, this);
            boxLayout->addWidget(packageWidget);
        }
    } else {
        auto emptyLabel = new QLabel;
        emptyLabel->setText(tr("All is done"));
        emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
        boxLayout->addWidget(emptyLabel);
    }
}
