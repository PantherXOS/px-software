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

#include "DataEntities.h"

using namespace PKG;

class ScreenshotItem : public QListWidgetItem{
public:
    ScreenshotItem(Package *package, int id,QListWidget *parent = nullptr) : QListWidgetItem(parent){
        this->package=package;
        this->id = id;
    }

    void loadImage(const QString &localfile,const QSize &size){
        QIcon qicon;
        QImage image(localfile);
        qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
        setIcon(qicon.pixmap(size, QIcon::Normal, QIcon::On));
        imageFile = localfile;
    }

    QPixmap getPixMap(){
        QIcon qicon;
        QImage image(imageFile);
        qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
        pixmap = qicon.pixmap(image.size(), QIcon::Normal, QIcon::On);
        return pixmap;
    }

    int getId(){
        return id;
    }

    Package *getPackage(){
        return package;
    }

private:
    int id;
    Package *package;
    QPixmap pixmap;
    QString imageFile;
};

#endif //PX_SOFTWARE_SCREENSHOTITEM_H
