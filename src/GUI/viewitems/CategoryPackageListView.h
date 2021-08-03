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

#ifndef PX_SOFTWARE_CATEGORY_PACKAGELISTWIDGET_H
#define PX_SOFTWARE_CATEGORY_PACKAGELISTWIDGET_H

#include <QScrollArea>
#include <QMovie>
#include <QListWidget>

#include "PackageListWidget.h"

class CategoryPackageListView : public PackageListWidget{
    Q_OBJECT
public:
    CategoryPackageListView(bool removeEnable, const QString &title, PXContentWidget *parent = nullptr) : PackageListWidget(title, parent) {
        connect(m_pkgMgr, SIGNAL(categoryPackagesReady(const QUuid &,const QVector<Package *> &)),this, SLOT(categoryPackagesReadyHandler(const QUuid &,const QVector<Package *> &)));
        this->removeEnable=removeEnable;
        m_pkgMgr->requestCategoryPackages(title);
    };

private slots:
    void categoryPackagesReadyHandler(const QUuid &taskId,const QVector<Package *> & packages){
        setLoadingVisible(false);
        setListVisible(true);
        for(auto pkg:packages) {
            auto *packageWidget = new PackageListWidgetItem1(pkg, true, removeEnable , this);
            addItem(packageWidget);
        }
    }

    void taskFailedHandler(const QUuid &taskId, const QString & message) {
        qDebug() << this << " : "<< message;
    }

private:
    bool removeEnable;
};


#endif //PX_SOFTWARE_CATEGORY_PACKAGELISTWIDGET_H
