//
// Created by hamzeh on 12/21/19.
//

#ifndef PX_SOFTWARE_SCREENSHOTVIEWER_H
#define PX_SOFTWARE_SCREENSHOTVIEWER_H

#include <QLabel>
#include <QIcon>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "PxQScrollArea.h"
#include "PackageManager.h"
#include "DataEntities.h"
#include "ScreenshotItem.h"
#include "FileDownloader.h"
#include "Settings.h"

using namespace PKG;

class ScreenShotViewer : public PxQScrollArea{
    Q_OBJECT
public:
    ScreenShotViewer(ScreenshotItem *item, PxQScrollArea *parent = nullptr) : PxQScrollArea(
            item->getPackage()->name(), parent) {
        this->package = item->getPackage();
        currentIndex = item->getId();
        imageLebel = new QLabel;
        imageLebel->setAlignment(Qt::AlignCenter);
        setWidgetResizable(true);
        setWidget(imageLebel);

        showImage(currentIndex);
    }

    void showImage(int index){
        QUrl fileUrl = package->screenShots().at(index);

        const char *homedir = getpwuid(getuid())->pw_dir;
        QString imageFileLocalPath = QString(homedir) + QString(PACKAGE_SCREENSHOTS_CACHE_DIR) + QString(this->package->name()) + QString("/");
        QString imageFilePath = imageFileLocalPath + fileUrl.fileName();
        QFile imageFile(imageFilePath);
        if(!imageFile.exists()){
            FileDownloader *m_pImgCtrl = new FileDownloader(fileUrl,
                                            imageFileLocalPath,
                                            this);
            connect(m_pImgCtrl, SIGNAL (downloaded(const QString &)), this, SLOT (imageDownloaded(const QString &)));
        }
        imageDownloaded(imageFilePath);
    }

    QString getTitle() override {
        return this->package->name()+"/"+ QUrl(this->package->screenShots().at(currentIndex)).fileName();
    }
private slots:
    void imageDownloaded(const QString & localfile){
        QIcon qicon;
        QImage image(localfile);
        qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
        imageLebel->setPixmap(qicon.pixmap(image.size(), QIcon::Normal, QIcon::On));
        imageLebel->showMaximized();
    };

private:
    QLabel *imageLebel;
    Package *package;
    int currentIndex;
};

#endif //PX_SOFTWARE_SCREENSHOTVIEWER_H
