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

#ifndef PX_SOFTWARE_PXQSCROLLAREA_H
#define PX_SOFTWARE_PXQSCROLLAREA_H

#include <QWidget>
#include <QString>
#include <QScrollArea>
#include <QDebug>
#include <QtGui/QGuiApplication>

#include "Settings.h"

class PXScrollArea : public QScrollArea{
public:
    PXScrollArea(const QString &title, QScrollArea *parent = nullptr) : QScrollArea(parent){
        this->title=title;
        setWidgetResizable(true);
        auto pal = QGuiApplication::palette();
        auto bgcolor = pal.color(QPalette::Active, QPalette::Base);
        auto fgcolor = pal.color(QPalette::Active, QPalette::Text);
        QString sheet = QString::fromLatin1(CONTENT_SCROLLAREA_STYLE).arg(bgcolor.name(),fgcolor.name());
        setStyleSheet(sheet);
        setAutoFillBackground(true);
    }
    virtual QString getTitle() {return title;}
    
private:
    QString title;
};


#endif //PX_SOFTWARE_PXQSCROLLAREA_H
