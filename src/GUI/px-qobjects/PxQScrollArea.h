//
// Created by hamzeh on 10/30/19.
//

#ifndef PX_SOFTWARE_PXQSCROLLAREA_H
#define PX_SOFTWARE_PXQSCROLLAREA_H

#include <QWidget>
#include <QString>
#include <QScrollArea>
#include <QDebug>

#include "Settings.h"

class PxQScrollArea : public QScrollArea{
public:
    PxQScrollArea(const QString &title, QScrollArea *parent = nullptr) : QScrollArea(parent){
        this->title=title;
        setWidgetResizable(true);
        setStyleSheet(CONTENT_SCROLLAREA_STYLE);
    }
    virtual QString getTitle() {return title;}

private:
    QString title;
};


#endif //PX_SOFTWARE_PXQSCROLLAREA_H
