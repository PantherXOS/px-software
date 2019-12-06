//
// Created by hamzeh on 11/23/19.
//

#include "PackageDetails.h"
#define SCREENSHOTS_CACHE_DIR "/.cache/px/px-software/images/"
#define SCREENSHOT_WIDTH 640
#define SCREENSHOT_HIEGHT 480

PackageDetails::PackageDetails(Package *package, const QString &title, PxQScrollArea *parent) : PxQScrollArea(
        title, parent) {
    auto packageComponent = new PackageComponent(package,true,this);

    this->package = package;
    auto leftSide = new QVBoxLayout;
    leftSide->addLayout(packageComponent->getIconLayout());
    leftSide->addLayout(packageComponent->getButtonsLayoutAsDetails());

    auto rightSide = new QVBoxLayout;
    rightSide->addLayout(loadRightSide());

    auto layout = new QHBoxLayout;
    layout->addLayout(leftSide);
    layout->addLayout(rightSide);

    QWidget *widget=new QWidget(this);
    widget->setLayout(layout);
    setWidgetResizable(true);
    setWidget(widget);
}

QVBoxLayout *PackageDetails::loadRightSide() {
    QFont titleFont("default", 12,QFont::Bold);
    QFont descriptionFont("default", 10);
    // add title, license and desc
    QLabel *titleLabel= new QLabel(this->package->title(),this);
    titleLabel->setFont(titleFont);

    QLabel *descriptionLabel= new QLabel(this->package->description(),this);
    descriptionLabel->setFont(descriptionFont);

    QLabel *screenShotsLabel = new QLabel("Screen Shots",this);
    screenShotsLabel->setFont(titleFont);

    auto screenshotSeperator = new PxLineSeperator(this);

    auto screenshotList = new QListWidget;
    screenshotList->setViewMode(QListWidget::IconMode);
    screenshotList->setIconSize(QSize(SCREENSHOT_WIDTH,SCREENSHOT_HIEGHT));
    screenshotList->setResizeMode(QListWidget::Adjust);
    screenshotList->setAutoFillBackground(false);
    screenshotList->setStyleSheet("background-color: transparent;");
    screenshotList->setWrapping(false);
    screenshotList->setFixedHeight(SCREENSHOT_HIEGHT);
    connect(screenshotList, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(onScreenshotClicked(QListWidgetItem*)));
    for(const auto &scr: package->screenShots()){
        screenshotList->addItem(downloadScreenshots(scr));
    }
    auto screenShotLayout = new QHBoxLayout;
    screenShotLayout->addWidget(screenshotList);
    screenShotLayout->setAlignment(Qt::AlignTop|Qt::AlignLeft);

//    QLabel *tagsLabel = new QLabel("Tags",this);
//    tagsLabel->setFont(titleFont);
//    QString tags="";
//    for(const auto & t: package->tags())
//        tags+=t+", ";
//    QLabel *tagsValue = new QLabel(tags,this);

    auto textLayout = new QVBoxLayout;
    textLayout->addWidget(titleLabel);
    textLayout->addWidget(descriptionLabel);
    textLayout->addWidget(screenShotsLabel);
    textLayout->addWidget(screenshotSeperator);
    textLayout->addLayout(screenShotLayout);
//    textLayout->addWidget(tagsLabel);
//    textLayout->addWidget(tagsValue);
    textLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    textLayout->setSpacing(15);
    textLayout->setMargin(7);

    return textLayout;
}

ScreenshotItem * PackageDetails::downloadScreenshots(const QUrl &url) {
    auto scrItem = new ScreenshotItem;
    screenshotMap[url.fileName()]=scrItem;
    const char *homedir = getpwuid(getuid())->pw_dir;
    QString iconFileLocalPath = QString(homedir)+QString(SCREENSHOTS_CACHE_DIR)+QString(this->package->name())+QString("/");
    QFile iconFile(iconFileLocalPath+url.fileName());
    if(!iconFile.exists()){
        screenshotDownloader = new FileDownloader(url,
                                        iconFileLocalPath,
                                        this);
        connect(screenshotDownloader, SIGNAL (downloaded(const QString &)), this, SLOT (screenshotsDownloaded(const QString &)));
        return scrItem;
    }
    screenshotsDownloaded(iconFileLocalPath+url.fileName());
    return scrItem;
}

void PackageDetails::screenshotsDownloaded(const QString &localfile) {
    screenshotMap[QUrl(localfile).fileName()]->loadImage(localfile,QSize(SCREENSHOT_WIDTH,SCREENSHOT_HIEGHT));
}

void PackageDetails::onScreenshotClicked(QListWidgetItem *item) {
    auto screenshot = new QLabel;
    screenshot->setPixmap(((ScreenshotItem *)item)->getPixMap());
    screenshot->showMaximized();
    screenshot->setAlignment(Qt::AlignCenter);
    screenshot->show();
}