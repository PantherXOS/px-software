//
// Created by hamzeh on 12/3/19.
//

#ifndef PX_SOFTWARE_SCREENSHOTITEM_H
#define PX_SOFTWARE_SCREENSHOTITEM_H
#include <QListWidgetItem>
#include <QIcon>
#include <QPixmap>

#include "DataEntities.h"

using namespace PKG;

class ScreenshotItem : public QListWidgetItem{
public:
    ScreenshotItem(Package *package, int id,QListWidget *parent = nullptr) : QListWidgetItem(parent){
        this->package=package;
        this->id = id;
    }

    void loadImage(const QString &localfile,const QSize &size){
        QIcon qicon;
        QImage image(localfile);
        qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
        setIcon(qicon.pixmap(size, QIcon::Normal, QIcon::On));
        imageFile = localfile;
    }

    QPixmap getPixMap(){
        QIcon qicon;
        QImage image(imageFile);
        qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
        pixmap = qicon.pixmap(image.size(), QIcon::Normal, QIcon::On);
        return pixmap;
    }

    int getId(){
        return id;
    }

    Package *getPackage(){
        return package;
    }

private:
    int id;
    Package *package;
    QPixmap pixmap;
    QString imageFile;
};

#endif //PX_SOFTWARE_SCREENSHOTITEM_H
