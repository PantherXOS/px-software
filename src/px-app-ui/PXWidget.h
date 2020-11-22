//
// Created by hamzeh on 12/31/19.
//

#ifndef PX_SOFTWARE_PXWIDGET_H
#define PX_SOFTWARE_PXWIDGET_H
#include <QWidget>

#include "PXContentWidget.h"

class PXWidget : public QWidget{
    Q_OBJECT
public:
    PXWidget(QWidget *parent= nullptr) : QWidget(parent){
        setObjectName("PXWidget");
        setStyleSheet(PX_WIDGET_STYLE);
        setContentsMargins(10,10,10,10);
    }

    PXContentWidget *getContent(){
        return content;
    }

private:
    PXContentWidget *content = nullptr;
};

#endif //PX_SOFTWARE_PXWIDGET_H
