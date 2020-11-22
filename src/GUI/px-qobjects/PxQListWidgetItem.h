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

#ifndef PX_SETTINGS_UI_PXQLISTWIDGETITEM_H
#define PX_SETTINGS_UI_PXQLISTWIDGETITEM_H

#include <string>
#include <QObject>
#include <QWidget>
#include <QListWidgetItem>
#include <QFont>
#include <QIcon>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QBitmap>

#include "Settings.h"
#include "PXScrollArea.h"

using namespace std;
class PxQListWidgetItem : public QListWidgetItem
{
public:
    PxQListWidgetItem(const QString &id, const QString &title, const QFont &font, const QString &iconItemFile,
                      QListWidget *parent = nullptr) : QListWidgetItem(parent){
        auto titleLabel = new QLabel(title);
        titleLabel->setFont(font);
        auto pal = QGuiApplication::palette();
        auto fgcolor =  pal.color(QPalette::Active, QPalette::WindowText);
        QString sheet = QString::fromLatin1(CONTENT_LIST_ITEM_STYLE).arg(fgcolor.name());
        titleLabel->setStyleSheet(sheet);
        QIcon qicon(QIcon::fromTheme(iconItemFile));
        QPixmap pixmap = qicon.pixmap(QSize(CONTENT_LIST_ICON_SIZE, CONTENT_LIST_ICON_SIZE), QIcon::Normal, QIcon::On);
        auto iconItem = new QLabel;
        iconItem->setPixmap(pixmap);

        auto llayout = new QHBoxLayout;
        llayout->addWidget(iconItem);
        llayout->addWidget(titleLabel);
        llayout->setAlignment(Qt::AlignLeft);

        rlayout = new QHBoxLayout;
        rlayout->setAlignment(Qt::AlignRight);

        auto mainLayout = new QHBoxLayout;
        mainLayout->addLayout(llayout);
        mainLayout->addLayout(rlayout);
        mainLayout->setContentsMargins(10,10,mainLayout->contentsMargins().right(),5);

        customWidget = new QWidget;
        customWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        customWidget->setLayout(mainLayout);
    }

    QWidget *getCustomWidget(){
        return customWidget;
    }

    virtual PXScrollArea *getView(){
        return view;
    }

    QHBoxLayout *rightLayout (){
        return rlayout;
    }

private:
    QHBoxLayout *rlayout;
    QWidget *customWidget;
    PXScrollArea *view;
};

#endif //PX_SETTINGS_UI_PXQLISTWIDGETITEM_H
