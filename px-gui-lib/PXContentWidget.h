//
// Created by hamzeh on 12/31/19.
//

#ifndef PX_SOFTWARE_PXCONTENTWIDGET_H
#define PX_SOFTWARE_PXCONTENTWIDGET_H
#include <QScrollArea>

#include "PXParamSettings.h"

class PXContentWidget : public QScrollArea {
    Q_OBJECT
public:
    explicit PXContentWidget(QScrollArea *parent = nullptr) : QScrollArea(parent){
        // auto pal = QGuiApplication::palette();
        // auto bgcolor = pal.color(QPalette::Active, QPalette::Base);
        // auto fgcolor = pal.color(QPalette::Active, QPalette::Text);
        // QString sheet = QString::fromLatin1(CONTENT_SCROLLAREA_STYLE).arg(bgcolor.name(),fgcolor.name());
        // setStyleSheet(sheet);
        setObjectName("PXContentWidget");
        setStyleSheet(PX_CONTENT_WIDGET_STYLE);
        // setContentsMargins(10,10,10,10);
        setWidgetResizable(true);
        setAutoFillBackground(true);
    }
 
    explicit PXContentWidget(const QString &title, QScrollArea *parent = nullptr) : QScrollArea(parent){
        this->title=title;
        setObjectName("PXContentWidget");
        setStyleSheet(PX_CONTENT_WIDGET_STYLE);
        // setContentsMargins(10,10,10,10);
        setWidgetResizable(true);
        setAutoFillBackground(true);
    }

    void setTitle(const QString &_title){
        title = _title;
    }

    virtual QString getTitle() {return title;}

private:
    QString title;
};


#endif //PX_SOFTWARE_PXCONTENTWIDGET_H
