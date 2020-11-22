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
        setObjectName("PXContentWidget");
        setStyleSheet(PX_CONTENT_WIDGET_STYLE);
        setContentsMargins(10,10,10,10);
        setWidgetResizable(true);
    }

    explicit PXContentWidget(const QString &title, QScrollArea *parent = nullptr) : QScrollArea(parent){
        this->title=title;
        setObjectName("PXContentWidget");
        setStyleSheet(PX_CONTENT_WIDGET_STYLE);
        setContentsMargins(10,10,10,10);
        setWidgetResizable(true);
    }

    void setTitle(const QString &_title){
        title = _title;
    }

    virtual QString getTitle() {return title;}

private:
    QString title;
};


#endif //PX_SOFTWARE_PXCONTENTWIDGET_H
