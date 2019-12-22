//
// Created by hamzeh on 12/21/19.
//

#ifndef PX_SOFTWARE_SCREENSHOTVIEWER_H
#define PX_SOFTWARE_SCREENSHOTVIEWER_H

#include <QLabel>
#include <QIcon>
#include <QVBoxLayout>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <QPushButton>

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
        QSize size(SCREENSHOT_ARROW_KEY_ICON_SIZE,SCREENSHOT_ARROW_KEY_ICON_SIZE);
        // previous button
        prevButton = new QPushButton(this);
        prevButton->setFixedSize(size);
        prevButton->setIcon(QIcon(":/images/general/src/GUI/resources/screenshot_previous"));
        prevButton->setIconSize(size);
        prevButton->setStyleSheet(SCREENSHOT_ARROW_KEY_STYLESHEET);
        connect(prevButton, SIGNAL(released()), this, SLOT(prevButtonHandler()));

        auto prevLayout = new QHBoxLayout();
        prevLayout->addWidget(prevButton);
        prevLayout->setAlignment(Qt::AlignCenter);
        // image layout
        currentIndex = item->getId();
        imageLabel = new QLabel;
        imageLabel->setAlignment(Qt::AlignCenter | Qt::AlignLeft);

        auto imageLayout = new QHBoxLayout;
        imageLayout->addWidget(imageLabel);
        imageLayout->setAlignment(Qt::AlignCenter);
        // next button
        nextButton = new QPushButton(this);
        nextButton->setFixedSize(size);
        nextButton->setIcon(QIcon(":/images/general/src/GUI/resources/screenshot_next"));
        nextButton->setIconSize(size);
        nextButton->setStyleSheet(SCREENSHOT_ARROW_KEY_STYLESHEET);
        connect(nextButton, SIGNAL(released()), this, SLOT(nextButtonHandler()));

        auto nextLayout = new QHBoxLayout();
        nextLayout->addWidget(nextButton);
        nextLayout->setAlignment(Qt::AlignCenter|Qt::AlignRight);
        // main layout
        auto mainLayout = new QHBoxLayout;
        mainLayout->addLayout(prevLayout);
        mainLayout->addLayout(imageLayout);
        mainLayout->addLayout(nextLayout);

        auto widget = new QWidget;
        widget->setLayout(mainLayout);
        setWidgetResizable(true);
        setWidget(widget);
        showImage(currentIndex);
    }

    void showImage(int index){
        QUrl fileUrl = package->screenShots().at(index);
        const char *homedir = getpwuid(getuid())->pw_dir;
        QString imageFileLocalPath = QString(homedir) + QString(PACKAGE_SCREENSHOTS_CACHE_DIR) + QString(this->package->name()) + QString("/");
        QString imageFilePath = imageFileLocalPath + fileUrl.fileName();
        QFile imageFile(imageFilePath);
        if(!imageFile.exists()){
            auto *m_pImgCtrl = new FileDownloader(fileUrl,
                                            imageFileLocalPath,
                                            this);
            connect(m_pImgCtrl, SIGNAL (downloaded(const QString &)), this, SLOT (imageDownloaded(const QString &)));
        }
        imageDownloaded(imageFilePath);
        currentIndex = index;
        reloadButtons();
    }

    QString getTitle() override {
        return this->package->name()+"/"+ QUrl(this->package->screenShots().at(currentIndex)).fileName();
    }

    void reloadButtons(){
        prevButton->setDisabled(currentIndex == 0);
        nextButton->setDisabled(currentIndex >= (package->screenShots().size()-1));
    }

private slots:
    void nextButtonHandler(){
        int idx = currentIndex+1;
        showImage(idx);
    }

    void prevButtonHandler(){
        int idx = currentIndex-1;
        showImage(idx);
    }
    void imageDownloaded(const QString & localfile){
        QIcon qicon;
        QImage image(localfile);
        qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
        imageLabel->setPixmap(qicon.pixmap(image.size(), QIcon::Normal, QIcon::On));
        imageLabel->showMaximized();
    };

private:
    QLabel *imageLabel;
    QPushButton *nextButton, *prevButton;
    Package *package;
    int currentIndex;
};

#endif //PX_SOFTWARE_SCREENSHOTVIEWER_H
