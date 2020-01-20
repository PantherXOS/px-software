//
// Created by hamzeh on 1/5/20.
//

#ifndef PX_SOFTWARE_PXQWIDGET_H
#define PX_SOFTWARE_PXQWIDGET_H
#include <QWidget>
#include <QtGui/QGuiApplication>
#include <QDebug>
#include "Settings.h"

class PxQWidget : public QWidget{
public:
    PxQWidget(QWidget *parent = nullptr) : QWidget(parent) {
        auto pal = QGuiApplication::palette();
        auto bgcolor = pal.color(QPalette::Active, QPalette::Base);
        auto fgcolor = pal.color(QPalette::Active, QPalette::Text);
        QString sheet = QString::fromLatin1(CONTENT_WIDGET_STYLE).arg(bgcolor.name(),fgcolor.name());
        setStyleSheet(sheet);
    }
};
#endif //PX_SOFTWARE_PXQWIDGET_H
