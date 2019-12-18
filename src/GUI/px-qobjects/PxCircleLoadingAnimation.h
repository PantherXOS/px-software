//
// Created by hamzeh on 12/18/19.
//

#ifndef PX_SOFTWARE_PXCIRCLELOADINGANIMATION_H
#define PX_SOFTWARE_PXCIRCLELOADINGANIMATION_H

#include <QLabel>
#include <QMovie>


class PxCircleLoadingAnimation : public QLabel {
public:
    explicit PxCircleLoadingAnimation(QSize size, QWidget *parent = nullptr) : QLabel(parent){
        auto movie = new QMovie(":images/general/src/GUI/resources/circleloading.gif");
        movie->setScaledSize(size);
        setAlignment(Qt::AlignCenter);
        setMovie(movie);
        setFixedSize(size);
        movie->start();
    }
};
#endif //PX_SOFTWARE_PXCIRCLELOADINGANIMATION_H
