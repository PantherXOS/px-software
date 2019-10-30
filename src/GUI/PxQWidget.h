//
// Created by hamzeh on 10/30/19.
//

#ifndef PX_SOFTWARE_PXQWIDGET_H
#define PX_SOFTWARE_PXQWIDGET_H

#include <QWidget>

class PxQWidget : public QWidget{
public:
    PxQWidget(int id, QString title){
        this->id=id;
        this->title=title;
    }

    int getId(){ return id;}
    QString getTitle() {return title;}

private:
    int id;
    QString title;
};


#endif //PX_SOFTWARE_PXQWIDGET_H
