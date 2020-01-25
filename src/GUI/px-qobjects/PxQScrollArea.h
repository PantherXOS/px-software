//
// Created by hamzeh on 10/30/19.
//

#ifndef PX_SOFTWARE_PXQSCROLLAREA_H
#define PX_SOFTWARE_PXQSCROLLAREA_H

#include <QWidget>
#include <QString>
#include <QScrollArea>
#include <QDebug>
#include <QtGui/QGuiApplication>

#include "Settings.h"

class PxQScrollArea : public QScrollArea{
public:
    PxQScrollArea(const QString &title, QScrollArea *parent = nullptr) : QScrollArea(parent){
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
