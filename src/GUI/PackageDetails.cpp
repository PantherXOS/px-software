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

#include "PackageDetails.h"


PackageDetails::PackageDetails(Package *package, const QString &title, PXContentWidget *parent) : PXContentWidget(
        title, parent) {
    auto packageComponent = new PackageComponent(package,this);
    connect(packageComponent, SIGNAL(showTerminalSignal(TerminalWidget *)),this, SLOT(showTerminalSignalHandler(TerminalWidget *)));

    this->package = package;
    auto leftSide = new QVBoxLayout;
    auto iconLayout = packageComponent->getIconLayout();
    iconLayout->setContentsMargins(20,20,20,20);
    leftSide->addLayout(iconLayout);
    leftSide->addLayout(packageComponent->getButtonsLayoutAsDetails());

    auto rightSide = new QVBoxLayout;
    rightSide->addLayout(loadRightSide());

    auto layout = new QHBoxLayout;
    layout->addLayout(leftSide);
    layout->addLayout(rightSide);
    setAutoFillBackground(true);

    setLayout(layout);
    setWidgetResizable(true);
}

void PackageDetails::showTerminalSignalHandler(TerminalWidget *terminal){
    emit showTerminalSignal(terminal);
}

QVBoxLayout *PackageDetails::loadRightSide() {
    QFont titleFont("default", PACKAGE_TITLE_FONT_SIZE,QFont::Bold);
    QFont descriptionFont("default", PACKAGE_DESC_FONT_SIZE);
    // add title, license and desc
    QLabel *titleLabel= new QLabel(this->package->title(),this);
    titleLabel->setFont(titleFont);
    
    QLabel *descriptionLabel= new QLabel(this->package->description(),this);
    descriptionLabel->setFont(descriptionFont);

    QLabel *screenShotsLabel = new QLabel("Preview",this);
    screenShotsLabel->setFont(titleFont);

    auto screenshotSeperator = new PXSeperator(this);

    auto screenshotList = createScreenshotList(package->screenShots());
    auto screenShotLayout = new QHBoxLayout;
    screenShotLayout->addWidget(screenshotList);
    screenShotLayout->setAlignment(Qt::AlignCenter|Qt::AlignLeft);

    auto textLayout = new QVBoxLayout;
    textLayout->addWidget(titleLabel);
    textLayout->addWidget(descriptionLabel);
    textLayout->addWidget(screenShotsLabel);
    textLayout->addWidget(screenshotSeperator);
    textLayout->addLayout(screenShotLayout);
    textLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    textLayout->setSpacing(PACKAGE_DETAILS_RIGHT_PANEL_SPACE);
    textLayout->setMargin(PACKAGE_DETAILS_RIGHT_PANEL_MARGIN);

    return textLayout;
}

QListWidget *PackageDetails::createScreenshotList(const QStringList &list) {
    auto screenshotList = new QListWidget;
    screenshotList->setViewMode(QListWidget::IconMode);
    screenshotList->setResizeMode(QListWidget::Adjust);
    screenshotList->setAutoFillBackground(false);
    screenshotList->setWrapping(false);
    screenshotList->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    screenshotList->setStyleSheet(SCREENSHOT_LIST_STYLESHEET);
    screenshotList->setSpacing(PACKAGE_SCREENSHOT_SPACING);
    if(list.size()){
        connect(screenshotList, SIGNAL(itemClicked(QListWidgetItem*)),
                this, SLOT(onScreenshotClicked(QListWidgetItem*)));
        int i=0;
        for(auto const &l :list){
            QUrl url(l);
            auto scrItem = new ScreenshotItem(package,i++,QSize(PACKAGE_SCREENSHOT_W, PACKAGE_SCREENSHOT_H),screenshotList);
            screenshotMap[url.fileName()]=scrItem;
            QString iconFileLocalPath = CacheManager::instance()->cacheDir()+PACKAGE_SCREENSHOTS_CACHE_DIR + QString(this->package->name()) + QString("/");
            screenshotDownloader = new FileDownloader(this);
            connect(screenshotDownloader, SIGNAL (downloaded(const QString &)), this, SLOT (screenshotsDownloaded(const QString &)));
            screenshotDownloader->start(url,iconFileLocalPath);
            screenshotList->addItem(scrItem);
            screenshotList->setItemWidget(scrItem,scrItem->widget());
        }
    } else {
        // "no images found" if screen shot list is empty
        auto label = new QLabel(SCREENSHOT_NOT_AVAILABLE_MSG,this);
        auto font = label->font();
        font.setPointSize(SCREENSHOT_MSG_FONT_SIZE);
        label->setFont(font);
        
        auto widgetItem = new QListWidgetItem(screenshotList);
        widgetItem->setSizeHint(label->size());
        screenshotList->setItemWidget(widgetItem, label);
    }
    return screenshotList;
}

void PackageDetails::screenshotsDownloaded(const QString &localfile) {
    screenshotMap[QUrl(localfile).fileName()]->loadImage(localfile,QSize(PACKAGE_SCREENSHOT_W, PACKAGE_SCREENSHOT_H));
}

void PackageDetails::onScreenshotClicked(QListWidgetItem *item) {
    emit screenshotItemClicked((ScreenshotItem *)item);
}