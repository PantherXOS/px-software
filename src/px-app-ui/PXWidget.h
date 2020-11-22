//
// Created by hamzeh on 12/31/19.
//

#ifndef PX_SOFTWARE_PXWIDGET_H
#define PX_SOFTWARE_PXWIDGET_H
#include <QWidget>

#include "PXContentWidget.h"
#include <QGuiApplication>

class PXWidget : public QWidget{
    Q_OBJECT
public:
    PXWidget(QWidget *parent= nullptr) : QWidget(parent){
        // setObjectName("PXWidget");
        auto pal = QGuiApplication::palette();
        auto bgcolor = pal.color(QPalette::Active, QPalette::Base);
        auto fgcolor = pal.color(QPalette::Active, QPalette::Text);
        QString sheet = QString::fromLatin1(PX_WIDGET_STYLE).arg(bgcolor.name(),fgcolor.name());
        setStyleSheet(sheet);
        setAutoFillBackground(true);
    }

    PXContentWidget *getContent(){
        return content;
    }

private:
    PXContentWidget *content = nullptr;
};

#endif //PX_SOFTWARE_PXWIDGET_H
