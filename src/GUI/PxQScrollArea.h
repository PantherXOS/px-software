//
// Created by hamzeh on 10/30/19.
//

#ifndef PX_SOFTWARE_PXQSCROLLAREA_H
#define PX_SOFTWARE_PXQSCROLLAREA_H

#include <QWidget>
#include <QString>
#include <QScrollArea>

class PxQScrollArea : public QScrollArea{
public:
    PxQScrollArea(int id, QString title){
        this->id=id;
        this->title=title;
        setWidgetResizable(true);
    }

    int getId(){ return id;}
    QString getTitle() {return title;}

private:
    int id;
    QString title;
};


#endif //PX_SOFTWARE_PXQSCROLLAREA_H
