//
// Created by hamzeh on 12/16/19.
//

#ifndef PX_SOFTWARE_PXVIEWLOADINGANIMATION_H
#define PX_SOFTWARE_PXVIEWLOADINGANIMATION_H

#include <QLabel>
#include <QMovie>

#include "Settings.h"

class PxViewLoadingAnimation : public QLabel {
public:
    explicit PxViewLoadingAnimation(QWidget *parent = nullptr) : QLabel(parent){
        auto movie = new QMovie(":images/general/src/GUI/resources/loading.gif");
        QSize size(VIEW_LOADING_ICON_SIZE,VIEW_LOADING_ICON_SIZE);
        movie->setScaledSize(size);
        setAlignment(Qt::AlignCenter);
        setMovie(movie);
        setFixedSize(size);
        movie->start();
    }
};
#endif //PX_SOFTWARE_PXVIEWLOADINGANIMATION_H
