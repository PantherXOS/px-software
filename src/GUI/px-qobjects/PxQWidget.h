//
// Created by hamzeh on 1/5/20.
//

#ifndef PX_SOFTWARE_PXQWIDGET_H
#define PX_SOFTWARE_PXQWIDGET_H
#include <QWidget>
#include "Settings.h"

class PxQWidget : public QWidget{
public:
    PxQWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setStyleSheet(CONTENT_WIDGET_STYLE);
        setContentsMargins(10,10,10,10);
    }
};
#endif //PX_SOFTWARE_PXQWIDGET_H
