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
#include "PackageListWidgetItem.h"
#include "PXScrollArea.h"
#include "PackageManager.h"
#include "QProgressIndicator.h"

using namespace PKG;
class TagPackageList : public PXScrollArea {
Q_OBJECT
public:
    TagPackageList(const QString &title, const QString &tag, PXScrollArea *parent = nullptr) : PXScrollArea(title, parent) {
        this->tag=tag;
        PackageManager *m_pkgMgr = PackageManager::Instance();
        connect(m_pkgMgr, SIGNAL(taskFailed(
                                         const QUuid &, const QString &)), this, SLOT(taskFailedHandler(
                                                                                              const QUuid &, const QString &)));
        connect(m_pkgMgr, SIGNAL(tagPackagesReady(
                                         const QUuid &, const QVector<Package *> &)), this,
                SLOT(tagPackagesReadyHandler(
                             const QUuid &, const QVector<Package *> &)));

        auto loading = new QProgressIndicator(this);
        loading->setFixedSize(VIEW_LOADING_ICON_SIZE,VIEW_LOADING_ICON_SIZE);
        loading->startAnimation();

        boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        boxLayout->setAlignment(Qt::AlignCenter);
        boxLayout->addWidget(loading);
        auto *widget=new PXWidget;
        widget->setLayout(boxLayout);
        setWidgetResizable(true);
        setWidget(widget);
        taskId = m_pkgMgr->requestTagPackages(tag);
    };

private slots:

    void tagPackagesReadyHandler(const QUuid &_taskId, const QVector<Package *> &packages) {
        if(_taskId == taskId){
            boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
            boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            auto *widget=new PXWidget;
            widget->setLayout(boxLayout);
            setWidgetResizable(true);
            setWidget(widget);
            if(packages.size()){
                for (auto pkg:packages) {
                    auto packageWidget = new PackageListWidgetItem(pkg, false, this);
                    boxLayout->addWidget(packageWidget);
                }
            } else {
                auto emptyLabel = new QLabel;
                emptyLabel->setText(tr("No record found for Tag") + QString("=\"") + tag +"\"");
                emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
                boxLayout->addWidget(emptyLabel);
            }
        }
    }

    void taskFailedHandler(const QUuid &_taskId, const QString &message) {
        if(_taskId == taskId){
            auto emptyLabel = new QLabel;
            emptyLabel->setText(message);
            emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
            boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
            boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            boxLayout->addWidget(emptyLabel);
            auto *widget=new PXWidget;
            widget->setLayout(boxLayout);
            setWidgetResizable(true);
            setWidget(widget);
        }
    }

private:
    QBoxLayout *boxLayout = nullptr;
    QString tag;
    QUuid taskId;
};

#endif //PX_SOFTWARE_TAGPACKAGELIST_H
