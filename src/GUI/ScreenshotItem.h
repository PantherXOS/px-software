//
// Created by hamzeh on 12/3/19.
//

#ifndef PX_SOFTWARE_SCREENSHOTITEM_H
#define PX_SOFTWARE_SCREENSHOTITEM_H
#include <QListWidgetItem>
#include <QIcon>
#include <QPixmap>

class ScreenshotItem : public QListWidgetItem{

#define SCREENSHOT_WIDTH 640
#define SCREENSHOT_HIEGHT 480

public:
    void loadImage(const QString &localfile){
        QIcon qicon;
        QImage image(localfile);
        qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
        setIcon(qicon.pixmap(QSize(SCREENSHOT_WIDTH,SCREENSHOT_HIEGHT), QIcon::Normal, QIcon::On));
        imageFile = localfile;
    }

    QPixmap getPixMap(){
        QIcon qicon;
        QImage image(imageFile);
        qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
        pixmap = qicon.pixmap(image.size(), QIcon::Normal, QIcon::On);
        return pixmap;
    }

private:
    QPixmap pixmap;
    QString imageFile;
};
#endif //PX_SOFTWARE_SCREENSHOTITEM_H
