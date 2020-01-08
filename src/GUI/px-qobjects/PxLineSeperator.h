//
// Created by hamzeh on 11/26/19.
//

#ifndef PX_SOFTWARE_PXLINESEPERATOR_H
#define PX_SOFTWARE_PXLINESEPERATOR_H
#include <QFrame>

using namespace std;
class PxLineSeperator : public QFrame{
    Q_OBJECT
public:
    PxLineSeperator(QWidget *parent = nullptr) : QFrame(parent){
        setGeometry(QRect(320, 150, 118, 1));
        setFrameShape(QFrame::HLine);
        setFrameShadow(QFrame::Sunken);
    }
};
#endif //PX_SOFTWARE_PXLINESEPERATOR_H
