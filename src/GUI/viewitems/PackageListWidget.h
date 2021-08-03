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
#include <QListWidget>

#include "PXContentWidget.h"
#include "PackageListWidgetItem.h"
#include "PackageManager.h"
#include "PXProgressIndicator.h"
#include "OtherApplicationsWidgetItem.h"

class PackageListWidget : public PXContentWidget{
    Q_OBJECT
public:
    PackageListWidget(const QString &title, PXContentWidget *parent = nullptr) : PXContentWidget(title, parent) {
        m_pkgMgr = PackageManager::Instance();
        connect(m_pkgMgr, SIGNAL(taskFailed(const QUuid &, const QString &)),this, SLOT(taskFailedHandler(const QUuid &, const QString &)));
        _loading = new PXProgressIndicator(this);
        _loading->setFixedSize(VIEW_LOADING_ICON_SIZE,VIEW_LOADING_ICON_SIZE);
        _loading->startAnimation();

        _listWidget = new QListWidget(this);
        _listWidget->setVisible(false);
        connect(_listWidget, &QListWidget::itemPressed, [&](QListWidgetItem *item){
            auto packageItem = (PackageListWidgetItem1*)item;
            auto pkg = packageItem->widget()->getPackage();
            qDebug() << pkg->name();
            emit packageItemClicked(pkg);
        });

        auto boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        boxLayout->setAlignment(Qt::AlignCenter);
        boxLayout->addWidget(_loading);
        boxLayout->addWidget(_listWidget);

        setWidgetResizable(true);
        setLayout(boxLayout);
    };

protected:
    void setLoadingVisible(bool visibility){
        _loading->setVisible(visibility);
    }

    void setListVisible(bool visibility){
        _listWidget->setVisible(visibility);
    }

    void addItem(QWidget *widget) {
        QListWidgetItem *item = new QListWidgetItem();
        _listWidget->addItem(item);
        _listWidget->setItemWidget(item, widget);
        item->setSizeHint(widget->size());
    }

    void addItem(OtherApplicationsWidgetItem1 *item) {
        _listWidget->addItem(item);
        _listWidget->setItemWidget(item, item->widget());
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        item->setSizeHint(item->widget()->size());
    }

    void addItem(PackageListWidgetItem1 *item) {
        _listWidget->addItem(item);
        _listWidget->setItemWidget(item, item->widget());
        item->setSizeHint(item->widget()->size());
    }
signals:
    void packageItemClicked(PKG::Package *package);

private slots:
    virtual void taskFailedHandler(const QUuid &taskId, const QString & message) {
        qDebug() << this << " : "<< message;
    }

private:
    PXProgressIndicator *_loading = nullptr;
    QListWidget *_listWidget = nullptr;

protected:
    PackageManager *m_pkgMgr;
};


#endif //PX_SOFTWARE_PACKAGELISTWIDGET_H
