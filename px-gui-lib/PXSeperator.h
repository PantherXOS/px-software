//
// Created by hamzeh on 11/26/19.
//

#ifndef PX_SOFTWARE_PXSEPERATOR_H
#define PX_SOFTWARE_PXSEPERATOR_H
#include <QFrame>

using namespace std;
class PXSeperator : public QFrame{
    Q_OBJECT
public:
    PXSeperator(QWidget *parent = nullptr) : QFrame(parent){
        setGeometry(QRect(320, 150, 118, 1));
        setFrameShape(QFrame::HLine);
        setFrameShadow(QFrame::Sunken);
        setStyleSheet("QFrame{background-color: transparent}");
    }
};
#endif //PX_SOFTWARE_PXSEPERATOR_H
