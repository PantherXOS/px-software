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

#ifndef PX_SOFTWARE_SCREENSHOTITEM_H
#define PX_SOFTWARE_SCREENSHOTITEM_H
#include <QListWidgetItem>
#include <QIcon>
#include <QPixmap>
#include <QPaintEngine>
#include <QPainter>

#include "DataEntities.h"
#include "PXProgressIndicator.h"
#include "Settings.h"
#include "QGuiApplication"

using namespace PKG;

class ScreenshotItem : public QListWidgetItem{
public:
    ScreenshotItem(Package *package, int id, const QSize &size, QListWidget *parent = nullptr) : 
        QListWidgetItem(parent),
        _id(id),
        _package(package){

        _progressIndicator = new PXProgressIndicator();
        _progressIndicator->setFixedSize(PACKAGE_SCREENSHOT_LOADING_SIZE,PACKAGE_SCREENSHOT_LOADING_SIZE);
        _progressIndicator->startAnimation();
        _iconLabel = new QLabel();
        _iconLabel->setVisible(false);

        auto layout = new QHBoxLayout();
        layout->addWidget(_progressIndicator);
        layout->addWidget(_iconLabel);
        layout->setAlignment(Qt::AlignCenter);
        _widget = new QWidget();
        _widget->setLayout(layout);
        auto pal = QGuiApplication::palette();
        auto bgcolor =  pal.color(QPalette::Normal, QPalette::Highlight);
        _widget->setStyleSheet( QString::fromLatin1(ITEM_HOVER_STYLESHEET).arg(bgcolor.name()));

        setSizeHint(size);
    }

    void loadImage(const QString &localfile,const QSize &size){
        QIcon qicon;
        QImage image(localfile);
        qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
        QPixmap pixmap = qicon.pixmap(size, QIcon::Normal, QIcon::On);
        _iconLabel->setPixmap(pixmap);

        _progressIndicator->stopAnimation();
        _progressIndicator->setVisible(false);
        _progressIndicator->deleteLater();
        _iconLabel->setVisible(true);
    }

    QWidget *widget(){
        return _widget;
    }

    int getId(){
        return _id;
    }

    Package *getPackage(){
        return _package;
    }

private:
    int _id;
    Package *_package;
    PXProgressIndicator *_progressIndicator;
    QLabel  *_iconLabel;
    QWidget *_widget;
};

#endif //PX_SOFTWARE_SCREENSHOTITEM_H
