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

#include "SystemUpdatablePackageListView.h"
#include "OtherApplicationsWidgetItem.h"

SystemUpdatablePackageListView *SystemUpdatablePackageListView::_instance = nullptr;

SystemUpdatablePackageListView *SystemUpdatablePackageListView::Instance() {
    if(_instance==nullptr){
        qCritical() << "UserUpdatablePackageListView is not initiated";
    }
    return _instance;
}

void SystemUpdatablePackageListView::init(const QString &title) {
    if(_instance==nullptr)
        _instance = new SystemUpdatablePackageListView(title, nullptr);
}

void SystemUpdatablePackageListView::getSystemUpdatablePackages(const QVector<Package *> &packageList) {
    boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    auto widget=new QWidget(this);
    widget->setLayout(boxLayout);
    setWidgetResizable(true);
    setWidget(widget);
    if(packageList.size()){
        PackageListWidgetItem *firstPackage = nullptr;
        QVector<Package *> otherPackageList;
        for(auto &pkg:packageList) {
            if(pkg->isAvailableInDB()) {
                PackageListWidgetItem *packageWidget;
                if(firstPackage)
                    packageWidget = new PackageListWidgetItem(pkg, false, false, this);
                else {
                    // Disable all UPDATE button and show [UPDATE ALL] on first package
                    packageWidget = new PackageListWidgetItem(pkg, true, false, this);
                    firstPackage = packageWidget;
                }
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
            PackageListWidgetItem *packageWidget;
            if(firstPackage != nullptr)
                packageWidget = new PackageListWidgetItem(pkg, false, false, this);
            else {
                // Disable all UPDATE button and show [UPDATE ALL] on first package
                packageWidget = new PackageListWidgetItem(pkg, true, false, this);
                firstPackage = packageWidget;
            }
            boxLayout->addWidget(packageWidget);
        }
        firstPackage->enableUpdateAllButton();
    } else {
        auto emptyLabel = new QLabel;
        emptyLabel->setText(tr("Everything is up to date."));
        emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
        boxLayout->addWidget(emptyLabel);
    }
}

SystemUpdatablePackageListView::SystemUpdatablePackageListView(const QString &title,
                                                               PXContentWidget *parent) : PXContentWidget(title, parent) {
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    connect(m_pkgMgrTrk, SIGNAL(systemUpdatablePackageListReady(
                                        const QVector<Package *> &)), this, SLOT(getSystemUpdatablePackages(
                                                                                       const QVector<Package *> &)));
    connect(m_pkgMgrTrk, SIGNAL(taskFailed(const QUuid &,const QString &)),this, SLOT(taskFailedHandler(const QUuid &,const QString &)));
    connect(m_pkgMgrTrk, &PackageManagerTracker::systemUpdateFinished,[&](const QString &outData, const QString &errData){
        // refresh(); TODO
    });
}

void SystemUpdatablePackageListView::refresh() {
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
    taskId = m_pkgMgrTrk->requestSystemUpdatablePackageList();
}

void SystemUpdatablePackageListView::taskFailedHandler(const QUuid & _taskId, const QString &message) {
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