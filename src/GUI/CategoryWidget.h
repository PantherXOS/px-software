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

#ifndef PX_SOFTWARE_CATEGORYWIDGET_H
#define PX_SOFTWARE_CATEGORYWIDGET_H

#include <iostream>
#include <string>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLayout>
#include <QImage>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QPixmap>
#include <QDebug>
#include <QPushButton>
#include <QUrl>
#include <QRegExp>

#include "PXScrollArea.h"
#include "PxQPushButton.h"
#include "DataEntities.h"
#include "DataAccessLayer.h"
#include "PackageListWidgetItem.h"
#include "PackageListWidget.h"
#include "Settings.h"
#include "PXSeperator.h"
#include "CacheManager.h"

using namespace std;
using namespace PKG;
class CategoryWidget : public QWidget{
    Q_OBJECT
public:
    CategoryWidget(Category *category, QWidget *parent = nullptr);
    Category * getCategory();

private slots:
    void imageDownloaded(const QString & localfile);

protected:
    void paintEvent(QPaintEvent *);

private:
    void loadIcon();
    QLabel *iconButton;
    Category *category;
    FileDownloader *m_pImgCtrl;
    QString icon;
};


#endif //PX_SOFTWARE_CATEGORYWIDGET_H
