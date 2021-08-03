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

#ifndef PX_SOFTWARE_TAGPACKAGELIST_H
#define PX_SOFTWARE_TAGPACKAGELIST_H

#include <QMovie>
#include <QBoxLayout>
#include "QLabel"
#include "PackageListWidget.h"

using namespace PKG;
class TagPackageList : public PackageListWidget {
Q_OBJECT
public:
    TagPackageList(const QString &title, const QString &tag, PXContentWidget *parent = nullptr) : PackageListWidget(title, parent) {
        this->tag=tag;
        connect(m_pkgMgr, SIGNAL(tagPackagesReady(const QUuid &, const QVector<Package *> &)), this,
                          SLOT(tagPackagesReadyHandler(const QUuid &, const QVector<Package *> &)));
        taskId = m_pkgMgr->requestTagPackages(tag);
    };

private slots:
    void tagPackagesReadyHandler(const QUuid &_taskId, const QVector<Package *> &packages) {
        if(_taskId == taskId){
            setLoadingVisible(false);
            setListVisible(true);
            if(packages.size()){
                for (auto pkg:packages) {
                    auto packageWidget = new PackageListWidgetItem(pkg, true, false, this);
                    addItem(packageWidget);
                }
            } else {
                auto emptyLabel = new QLabel;
                emptyLabel->setText(tr("No record found for Tag") + QString("=\"") + tag +"\"");
                emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
                addItem(emptyLabel);
            }
        }
    }

    void taskFailedHandler(const QUuid &_taskId, const QString &message) override{
        if(_taskId == taskId){
            auto emptyLabel = new QLabel;
            emptyLabel->setText(message);
            emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
            setLoadingVisible(false);
            setListVisible(true);
            addItem(emptyLabel);
        }
    }

private:
    QString tag;
    QUuid taskId;
};

#endif //PX_SOFTWARE_TAGPACKAGELIST_H
