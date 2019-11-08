//
// Created by hamzeh on 11/3/19.
//

#ifndef PX_SOFTWARE_PXQPUSHBUTTON_H
#define PX_SOFTWARE_PXQPUSHBUTTON_H

#include <QPushButton>
#include <QString>

class PxQPushButton : public QPushButton{
    Q_OBJECT
public:
    void setKey(QString key){ this->key=key;}
    QString getKey(){ return this->key;}
private:
    QString key;
};
#endif //PX_SOFTWARE_PXQPUSHBUTTON_H
