//
// Created by hamzeh on 11/23/19.
//

#include "PackageDetails.h"
#define IMAGE_CACHE_DIR "/.cache/px/px-software/images/"
#define BUTTON_WIDTH 128
#define ICON_WIDTH 128

#define SCREENSHOT_WIDTH 640
#define SCREENSHOT_HIEGHT 480

PackageDetails::PackageDetails(Package *package, const QString &title, PxQScrollArea *parent) : PxQScrollArea(
        title, parent) {
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    connect(m_pkgMgrTrk, SIGNAL(packageUpdated(const QString)),this, SLOT(packageUpdatedHandler(const QString)));
    connect(m_pkgMgrTrk, SIGNAL(packageRemoved(const QString)),this, SLOT(packageRemovedHandler(const QString)));
    connect(m_pkgMgrTrk, SIGNAL(packageInstalled(const QString)),this, SLOT(packageInstalledHandler(const QString)));
    failedProgressConnection  = connect(m_pkgMgrTrk, SIGNAL(progressFailed(const QString&,const QString&)),this, SLOT(taskFailedHandler(const QString,const QString&)));

    this->package = package;
    auto leftSide = new QVBoxLayout;
    leftSide->addLayout(loadIcon(QUrl(package->icon())));
    leftSide->addLayout(loadButtons());

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

QHBoxLayout *PackageDetails::loadIcon(const QUrl &iconUrl) {
    iconButton = new QLabel(this);
    iconButton->setFixedSize(QSize(ICON_WIDTH,ICON_WIDTH));
    iconButton->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);;

    auto iconLayout = new QHBoxLayout;
    iconLayout->addWidget(iconButton);

    const char *homedir = getpwuid(getuid())->pw_dir;
    QString iconFileLocalPath = QString(homedir)+QString(IMAGE_CACHE_DIR)+QString(this->package->name())+QString("/");
    QFile iconFile(iconFileLocalPath+iconUrl.fileName());
    if(!iconFile.exists()){
        m_pImgCtrl = new FileDownloader(iconUrl,
                                        iconFileLocalPath,
                                        this);
        connect(m_pImgCtrl, SIGNAL (downloaded(const QString &)), this, SLOT (imageDownloaded(const QString &)));
        return iconLayout;
    }
    imageDownloaded(iconFileLocalPath+iconUrl.fileName());
    return iconLayout;
}


QVBoxLayout *PackageDetails::loadRightSide() {
    QFont titleFont("default", 12,QFont::Bold);
    QFont descriptionFont("default", 10);
    // add title, license and desc
    QLabel *titleLabel= new QLabel(this->package->title(),this);
    titleLabel->setFont(titleFont);

    QLabel *descriptionLabel= new QLabel(this->package->description(),this);
    descriptionLabel->setFont(descriptionFont);
//    auto descriptionScrollArea = new QScrollArea;
//    descriptionScrollArea->setFrameShape(QFrame::NoFrame);
//    descriptionScrollArea->setWidget(descriptionLabel);

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

QVBoxLayout * PackageDetails::loadButtons() {
    // add install,update and remove buttons
    auto buttonLayout = new QVBoxLayout;
    updateButton = new QPushButton(this);
    updateButton->setText("Update");
    updateButton->setFixedWidth(BUTTON_WIDTH);
    updateButton->setStyleSheet("QPushButton {background-color: green; color: white;}");
    connect(updateButton, SIGNAL(released()), this, SLOT(updateButtonHandler()));

    removeButton = new QPushButton(this);
    removeButton->setText("Remove");
    removeButton->setFixedWidth(BUTTON_WIDTH);
    removeButton->setStyleSheet("QPushButton {background-color: red; color: white;}");
    connect(removeButton, SIGNAL(released()), this, SLOT(removeButtonHandler()));

    installButton = new QPushButton(this);
    installButton->setText("Install");
    installButton->setFixedWidth(BUTTON_WIDTH);
    installButton->setStyleSheet("QPushButton {background-color: blue; color: white;}");
    connect(installButton, SIGNAL(released()), this, SLOT(installButtonHandler()));

    auto line = new PxLineSeperator(this);
    QLabel *version = new QLabel("Version : " + package->version(), this);
    QLabel *license = new QLabel("License : " + package->license(), this);

    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(installButton);
    buttonLayout->addWidget(line);
    buttonLayout->addWidget(version);
    buttonLayout->addWidget(license);

    reloadButtonsStatus();
    buttonLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    return buttonLayout;
}

void PackageDetails::reloadButtonsStatus() {
    updateButton->setVisible(false);
    removeButton->setVisible(false);
    installButton->setVisible(false);
    if(m_pkgMgrTrk->inInstalling(package->name())) {
        installButton->setText("Installing ...");
        installButton->setVisible(true);
    } else if(m_pkgMgrTrk->inRemoving(package->name())) {
        removeButton->setText("Removing ...");
        removeButton->setVisible(true);
    } else if(m_pkgMgrTrk->inUpdating(package->name())) {
        updateButton->setText("Updating ...");
        updateButton->setVisible(true);
    } else {
        if(package->isInstalled()) {
            if (package->isUpdateAvailable()) {
                updateButton->setText("Update");
                updateButton->setVisible(true);
            }
            removeButton->setText("Remove");
            removeButton->setVisible(true);
        } else {
            installButton->setText("Install");
            installButton->setVisible(true);
        }
    }
}

ScreenshotItem * PackageDetails::downloadScreenshots(const QUrl &url) {
    auto scrItem = new ScreenshotItem;
    screenshotMap[url.fileName()]=scrItem;
    const char *homedir = getpwuid(getuid())->pw_dir;
    QString iconFileLocalPath = QString(homedir)+QString(IMAGE_CACHE_DIR)+QString(this->package->name())+QString("/");
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

void PackageDetails::imageDownloaded(const QString & localfile){
    QIcon qicon;
    QImage image(localfile);
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
    QPixmap pixmap = qicon.pixmap(QSize(ICON_WIDTH,ICON_WIDTH), QIcon::Normal, QIcon::On);
    iconButton->setPixmap(pixmap);
}

void PackageDetails::installButtonHandler() {
    if(m_pkgMgrTrk->requestPackageInstallation(package->name()))
        installButton->setText("Installing ...");
}

void PackageDetails::removeButtonHandler() {
    if(m_pkgMgrTrk->requestPackageRemoval(package->name()))
        removeButton->setText("Removing ...");
}

void PackageDetails::updateButtonHandler() {
    if(m_pkgMgrTrk->requestPackageUpdate(package->name()))
        updateButton->setText("Updating ...");
}

void PackageDetails::packageUpdatedHandler(const QString &name) {
    if(name == package->name()){
        this->package->setUpdateAvailable(false);
        reloadButtonsStatus();
    }
}

void PackageDetails::packageRemovedHandler(const QString &name) {
    if(name == package->name()){
        this->package->setInstalled(false);
        reloadButtonsStatus();
    }
}

void PackageDetails::packageInstalledHandler(const QString &name) {
    if(name == package->name()){
        this->package->setInstalled(true);
        reloadButtonsStatus();
    }
}

void PackageDetails::taskFailedHandler(const QString &name, const QString &message) {
    if(name == package->name()){
        disconnect(failedProgressConnection);
        reloadButtonsStatus();
    }
}

void PackageDetails::onScreenshotClicked(QListWidgetItem *item) {
    auto screenshot = new QLabel;
    screenshot->setPixmap(((ScreenshotItem *)item)->getPixMap());
    screenshot->showMaximized();
    screenshot->setAlignment(Qt::AlignCenter);
    screenshot->show();
}