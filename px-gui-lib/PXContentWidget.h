//
// Created by hamzeh on 12/31/19.
//

#ifndef PX_SOFTWARE_PXCONTENTWIDGET_H
#define PX_SOFTWARE_PXCONTENTWIDGET_H
#include <QScrollArea>
#include <QGuiApplication>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

#include "PXParamSettings.h"
#include "PXWidget.h"

class PXContentWidget : public QScrollArea {
    Q_OBJECT
public:
    explicit PXContentWidget(QScrollArea *parent = nullptr) : QScrollArea(parent){
        // setObjectName("PXContentWidget");
        auto pal = QGuiApplication::palette();
        auto bgcolor = pal.color(QPalette::Active, QPalette::Base);
        auto fgcolor = pal.color(QPalette::Active, QPalette::Text);
        QString sheet = QString::fromLatin1(PX_CONTENT_WIDGET_STYLE).arg(bgcolor.name(),fgcolor.name());
        setStyleSheet(sheet);
        setWidgetResizable(true);
        setAutoFillBackground(true);
    }
 
    explicit PXContentWidget(const QString &title, QScrollArea *parent = nullptr) : QScrollArea(parent){
        this->title=title;
        // setObjectName("PXContentWidget");
        auto pal = QGuiApplication::palette();
        auto bgcolor = pal.color(QPalette::Active, QPalette::Base);
        auto fgcolor = pal.color(QPalette::Active, QPalette::Text);
        QString sheet = QString::fromLatin1(PX_CONTENT_WIDGET_STYLE).arg(bgcolor.name(),fgcolor.name());
        setStyleSheet(sheet);
        setWidgetResizable(true);
        setAutoFillBackground(true);
    }

    void setTitle(const QString &_title){
        title = _title;
    }

    virtual QString getTitle() {return title;}

    void setWidget(PXWidget *widget){
        QScrollArea::setWidget(widget);
    }

private:
    QString title;
};


#endif //PX_SOFTWARE_PXCONTENTWIDGET_H
