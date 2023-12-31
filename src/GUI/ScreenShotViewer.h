/*
 * px-software - Qt-GUI package manager for guix on PantherX OS
 * Copyright (C) 2019, 2020  Hamzeh Nasajpour <h.nasajpour@pantherx.org>
 * Copyright (C) 2019, 2020  Reza Alizadeh Majd <r.majd@pantherx.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef PX_SOFTWARE_SCREENSHOTVIEWER_H
#define PX_SOFTWARE_SCREENSHOTVIEWER_H

#include <QLabel>
#include <QIcon>
#include <QVBoxLayout>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <QPushButton>
#include <QKeyEvent>

#include "PXContentWidget.h"
#include "PackageManager.h"
#include "DataEntities.h"
#include "ScreenshotItem.h"
#include "FileDownloader.h"
#include "Settings.h"
#include "CacheManager.h"

using namespace PKG;

class ScreenShotViewer : public PXContentWidget{
    Q_OBJECT
public:
    ScreenShotViewer(ScreenshotItem *item, PXContentWidget *parent = nullptr) : PXContentWidget(
            item->getPackage()->name(), parent) {
        this->package = item->getPackage();
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        setAttribute(Qt::WA_NoSystemBackground);

        setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff );

        QSize size(SCREENSHOT_ARROW_KEY_ICON_SIZE,SCREENSHOT_ARROW_KEY_ICON_SIZE);
        // previous button
        prevButton = new QPushButton();
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
        imageLabel = new QLabel(this);
        imageLabel->setAlignment(Qt::AlignCenter | Qt::AlignLeft);
//        imageLabel->setSizePolicy(QSizePolicy::MinimumExpanding,
//                                  QSizePolicy::MinimumExpanding);
        imageLabel->setScaledContents(true);

        auto imageLayout = new QHBoxLayout;
        imageLayout->addWidget(imageLabel);
        imageLayout->setAlignment(Qt::AlignCenter);
        // next button
        nextButton = new QPushButton();
        nextButton->setFixedSize(size);
        nextButton->setIcon(QIcon(":/images/general/src/GUI/resources/screenshot_next"));
        nextButton->setIconSize(size);
        nextButton->setStyleSheet(SCREENSHOT_ARROW_KEY_STYLESHEET);
        connect(nextButton, SIGNAL(released()), this, SLOT(nextButtonHandler()));

        auto nextLayout = new QHBoxLayout();
        nextLayout->addWidget(nextButton);
        nextLayout->setAlignment(Qt::AlignCenter|Qt::AlignRight);
        // main layout
        auto mainLayout = new QHBoxLayout(this);
//        mainLayout->addLayout(prevLayout);
        mainLayout->addLayout(imageLayout);
//        mainLayout->addLayout(nextLayout);

        setLayout(mainLayout);
        showImage(currentIndex);
    }

    void showImage(int index){
        if((index >= 0) && (index < package->screenShots().size())) {
            FileDownloader::DownloadItem item;
            item.url = package->screenShots().at(index);            
            item.localFilePath = CacheManager::instance()->cacheDir()+PACKAGE_SCREENSHOTS_CACHE_DIR + QString(this->package->name()) + QString("/");
            item.localFileName = item.url.fileName();
            QFile imageFile(item.localFilePath + item.localFileName);
            if(!imageFile.exists()){
                auto *m_pImgCtrl = new FileDownloader(this);
                connect(m_pImgCtrl, SIGNAL (downloadComplete(const FileDownloader::DownloadItem&)), this, SLOT (imageDownloaded(const FileDownloader::DownloadItem &)));
                m_pImgCtrl->start(item);
            } else {
                imageDownloaded(item);
            }
            currentIndex = index;
            reloadButtons();
        }
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

    void imageDownloaded(const FileDownloader::DownloadItem& item){
        QPixmap image(item.localFilePath + item.localFileName);
        imageLabel->setPixmap(image.scaled(SCREENSHOT_PICTURE_SIZE_W, SCREENSHOT_PICTURE_SIZE_H, Qt::KeepAspectRatio));
    }

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() != Qt::Key_Escape) {
            if(e->type() == QEvent::KeyPress) {
                Qt::Key keyPressed = (Qt::Key)(e->key());
                if(keyPressed == Qt::Key_Left) {
                    prevButtonHandler();
                } else if (keyPressed == Qt::Key_Right) {
                    nextButtonHandler();
                } else {
                    PXContentWidget::keyPressEvent(e);
                }
            }
        } else {
            this->close();
        }
    }

private:
    QLabel *imageLabel;
    QPushButton *nextButton, *prevButton;
    Package *package;
    int currentIndex;
};

#endif //PX_SOFTWARE_SCREENSHOTVIEWER_H
