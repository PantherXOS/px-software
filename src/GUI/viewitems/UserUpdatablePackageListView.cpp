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

#include "UserUpdatablePackageListView.h"
#include "OtherApplicationsWidgetItem.h"

UserUpdatablePackageListView *UserUpdatablePackageListView::_instance = nullptr;

UserUpdatablePackageListView *UserUpdatablePackageListView::Instance() {
    if(_instance==nullptr){
        qCritical() << "UserUpdatablePackageListView is not initiated";
    }
    return _instance;
}

void UserUpdatablePackageListView::init(const QString &title) {
    if(_instance==nullptr)
        _instance = new UserUpdatablePackageListView(title, nullptr);
}

void UserUpdatablePackageListView::refresh() {
    auto loading = new PXProgressIndicator(this);
    loading->setFixedSize(VIEW_LOADING_ICON_SIZE,VIEW_LOADING_ICON_SIZE);
    loading->startAnimation();

    boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setAlignment(Qt::AlignCenter);
    boxLayout->addWidget(loading);
    auto widget=new QWidget(this);
    widget->setLayout(boxLayout);
    setWidgetResizable(true);
    setWidget(widget);
    taskId = m_pkgMgrTrk->requestUserUpdatablePackageList();
}

void UserUpdatablePackageListView::getUserUpdatablePackages(const QVector<Package *> &packageList) {
    boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    auto widget=new QWidget(this);
    widget->setLayout(boxLayout);
    setWidgetResizable(true);
    setWidget(widget);
    if(packageList.size()){
        QVector<Package *> otherPackageList;
        for(auto &pkg:packageList) {
            if(pkg->isAvailableInDB()) {
                auto packageWidget = new PackageListWidgetItem(pkg, true, true, this);
                boxLayout->addWidget(packageWidget);
            } else {
                otherPackageList.append(pkg);
            }
        }
        if(otherPackageList.size()) {
            auto otherApplicationTitle = new OtherApplicationsWidgetItem(this);
            boxLayout->addWidget(otherApplicationTitle);
        }
        for(auto &pkg:otherPackageList){
            auto packageWidget = new PackageListWidgetItem(pkg, true, true, this);
            boxLayout->addWidget(packageWidget);
        }
    } else {
        auto emptyLabel = new QLabel;
        emptyLabel->setText(tr("Everything is up to date."));
        emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
        boxLayout->addWidget(emptyLabel);
    }
}

UserUpdatablePackageListView::UserUpdatablePackageListView(const QString &title, PXContentWidget *parent)
        : PXContentWidget(title, parent) {
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    connect(m_pkgMgrTrk, SIGNAL(userUpdatablePackageListReady(
                                        const QVector<Package *> &)), this, SLOT(getUserUpdatablePackages(
                                                                                       const QVector<Package *> &)));
    connect(m_pkgMgrTrk, SIGNAL(taskFailed(const QUuid &,const QString &)),this, SLOT(taskFailedHandler(const QUuid &,const QString &)));
}

void UserUpdatablePackageListView::taskFailedHandler(const QUuid &_taskId, const QString &message) {
    if(_taskId == taskId){
        auto emptyLabel = new QLabel;
        emptyLabel->setText(message);
        emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
        boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        boxLayout->addWidget(emptyLabel);
        auto widget=new QWidget(this);
        widget->setLayout(boxLayout);
        setWidgetResizable(true);
        setWidget(widget);
    }
}