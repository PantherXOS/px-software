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

#ifndef PX_SOFTWARE_PACKAGELISTWIDGET_H
#define PX_SOFTWARE_PACKAGELISTWIDGET_H

#include <QScrollArea>
#include <QMovie>

#include "PxQWidget.h"
#include "PxQScrollArea.h"
#include "PackageListWidgetItem.h"
#include "PackageManager.h"
#include "QProgressIndicator.h"

class PackageListWidget : public PxQScrollArea{
    Q_OBJECT
public:
    PackageListWidget(bool removeEnable, const QString &title, PxQScrollArea *parent = nullptr) : PxQScrollArea(title, parent) {
        PackageManager *m_pkgMgr = PackageManager::Instance();
        connect(m_pkgMgr, SIGNAL(taskFailed(const QUuid &, const QString &)),this, SLOT(taskFailedHandler(const QUuid &, const QString &)));
        connect(m_pkgMgr, SIGNAL(categoryPackagesReady(const QUuid &,const QVector<Package *> &)),this, SLOT(categoryPackagesReadyHandler(const QUuid &,const QVector<Package *> &)));
        this->removeEnable=removeEnable;
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
        m_pkgMgr->requestCategoryPackages(title);
    };

private slots:
    void categoryPackagesReadyHandler(const QUuid &taskId,const QVector<Package *> & packages){
        if(boxLayout!=nullptr)
            delete boxLayout;

        boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        auto *widget=new PxQWidget;
        widget->setLayout(boxLayout);
        setWidgetResizable(true);
        setWidget(widget);
        for(auto pkg:packages) {
            PackageListWidgetItem *packageWidget = new PackageListWidgetItem(pkg, removeEnable , this);
            boxLayout->addWidget(packageWidget);
        }
    }

    void taskFailedHandler(const QUuid &taskId, const QString & message) {
        qDebug() << this << " : "<< message;
    }

private:
    QBoxLayout *boxLayout=nullptr;
    bool removeEnable;
};


#endif //PX_SOFTWARE_PACKAGELISTWIDGET_H
