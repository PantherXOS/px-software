//
// Created by hamzeh on 11/23/19.
//

#include "PackageDetails.h"
#define IMAGE_CACHE_DIR "/.cache/px/px-software/images/"
#define BUTTON_WIDTH 128
#define ICON_WIDTH 128
#define SCREENSHOT_WIDTH 256

PackageDetails::PackageDetails(Package *package, const int id, const QString &title, PxQScrollArea *parent) : PxQScrollArea(id, title, parent) {
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    connect(m_pkgMgrTrk, SIGNAL(packageUpdated(const QString)),this, SLOT(packageUpdatedHandler(const QString)));
    connect(m_pkgMgrTrk, SIGNAL(packageRemoved(const QString)),this, SLOT(packageRemovedHandler(const QString)));
    connect(m_pkgMgrTrk, SIGNAL(packageInstalled(const QString)),this, SLOT(packageInstalledHandler(const QString)));
    failedProgressConnection  = connect(m_pkgMgrTrk, SIGNAL(progressFailed(const QString&,const QString&)),this, SLOT(taskFailedHandler(const QString,const QString&)));

    this->package = package;
    QVBoxLayout *leftSide = new QVBoxLayout;
    leftSide->addLayout(loadIcon(QUrl(package->icon())));
    leftSide->addLayout(loadButtons());

    QVBoxLayout *rightSide = new QVBoxLayout;
    rightSide->addLayout(loadRightSide());

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(leftSide);
    layout->addLayout(rightSide);

    this->setLayout(layout);
}

QHBoxLayout *PackageDetails::loadIcon(const QUrl &iconUrl) {
    QHBoxLayout *iconLayout = new QHBoxLayout;
    const char *homedir = getpwuid(getuid())->pw_dir;
    QString iconFileLocalPath = QString(homedir)+QString(IMAGE_CACHE_DIR)+QString(this->package->name())+QString("/");
    QFile iconFile(iconFileLocalPath+iconUrl.fileName());
    if(!iconFile.exists()){
        m_pImgCtrl = new FileDownloader(iconUrl,
                                        iconFileLocalPath,
                                        this);
        connect(m_pImgCtrl, SIGNAL (downloaded(const QString &)), this, SLOT (imageDownloaded(const QString &)));
    }
    iconButton = new QLabel;
    QIcon qicon;
    QImage image(iconFileLocalPath+iconUrl.fileName());
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
    QPixmap pixmap = qicon.pixmap(QSize(ICON_WIDTH,ICON_WIDTH), QIcon::Normal, QIcon::On);
    iconButton->setPixmap(pixmap);
    iconButton->setFixedSize(QSize(ICON_WIDTH,ICON_WIDTH));
    iconButton->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    iconLayout->addWidget(iconButton);
    return iconLayout;
}


QVBoxLayout *PackageDetails::loadRightSide() {
    QFont titleFont("default", 12,QFont::Bold);
    QFont descriptionFont("default", 10);
    // add title, license and desc
    QLabel *titleLabel= new QLabel(this->package->title());
    titleLabel->setFont(titleFont);

    QLabel *descriptionLabel= new QLabel(this->package->description());
    descriptionLabel->setFont(descriptionFont);
    descriptionLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    descriptionLabel->setWordWrap(true);

    QLabel *screenShotsLabel = new QLabel("Screen Shots");
    screenShotsLabel->setFont(titleFont);

    QHBoxLayout *screenShotLayout = new QHBoxLayout;
    for(auto scr: package->screenShots()){
        QLabel *scrLabel = new QLabel;
        screenshotMap[QUrl(scr).fileName()]=scrLabel;
        screenShotLayout->addWidget(scrLabel);
        downloadScreenshots(scr);
    }

    QLabel *tagsLabel = new QLabel("Tags");
    tagsLabel->setFont(titleFont);
    QString tags="";
    for(const auto & t: package->tags())
        tags+=t+", ";
    QLabel *tagsValue = new QLabel(tags);

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->addWidget(titleLabel);
    textLayout->addWidget(descriptionLabel);
    textLayout->addWidget(screenShotsLabel);
    textLayout->addLayout(screenShotLayout);
    textLayout->addWidget(tagsLabel);
    textLayout->addWidget(tagsValue);

    textLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    return textLayout;
}

QVBoxLayout * PackageDetails::loadButtons() {
    // add install,update and remove buttons
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    updateButton = new QPushButton;
    updateButton->setText("Update");
    updateButton->setFixedWidth(BUTTON_WIDTH);
    updateButton->setStyleSheet("QPushButton {background-color: green; color: white;}");
    connect(updateButton, SIGNAL(released()), this, SLOT(updateButtonHandler()));

    removeButton = new QPushButton;
    removeButton->setText("Remove");
    removeButton->setFixedWidth(BUTTON_WIDTH);
    removeButton->setStyleSheet("QPushButton {background-color: red; color: white;}");
    connect(removeButton, SIGNAL(released()), this, SLOT(removeButtonHandler()));

    installButton = new QPushButton;
    installButton->setText("Install");
    installButton->setFixedWidth(BUTTON_WIDTH);
    installButton->setStyleSheet("QPushButton {background-color: blue; color: white;}");
    connect(installButton, SIGNAL(released()), this, SLOT(installButtonHandler()));

    upToDateButton = new QPushButton;
    upToDateButton->setText("Up-To-Date");
    upToDateButton->setFixedWidth(BUTTON_WIDTH);
    upToDateButton->setStyleSheet("QPushButton {background-color: gray; color: black;}");

    PxLineSeperator *line = new PxLineSeperator;
    QLabel *version = new QLabel("Version : " + package->version());
    QLabel *license = new QLabel("License : " + package->license());

    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(installButton);
    buttonLayout->addWidget(upToDateButton);
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
    upToDateButton->setVisible(false);
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
            if(!(package->isUpdateAvailable())) {
                upToDateButton->setText("Up-To-Date");
                upToDateButton->setVisible(true);
            }
        } else {
            installButton->setText("Install");
            installButton->setVisible(true);
        }
    }
}

void PackageDetails::downloadScreenshots(const QUrl &url) {
    const char *homedir = getpwuid(getuid())->pw_dir;
    QString iconFileLocalPath = QString(homedir)+QString(IMAGE_CACHE_DIR)+QString(this->package->name())+QString("/");
    QFile iconFile(iconFileLocalPath+url.fileName());
    if(!iconFile.exists()){
        m_pImgCtrl = new FileDownloader(url,
                                        iconFileLocalPath,
                                        this);
        connect(m_pImgCtrl, SIGNAL (downloaded(const QString &)), this, SLOT (screenshotsDownloaded(const QString &)));
    }
    QIcon qicon;
    QImage image(iconFileLocalPath+url.fileName());
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
    QPixmap pixmap = qicon.pixmap(QSize(SCREENSHOT_WIDTH,SCREENSHOT_WIDTH), QIcon::Normal, QIcon::On);
    screenshotMap[url.fileName()]->setPixmap(pixmap);
    screenshotMap[url.fileName()]->setFixedSize(QSize(SCREENSHOT_WIDTH,SCREENSHOT_WIDTH));
    screenshotMap[url.fileName()]->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

void PackageDetails::screenshotsDownloaded(const QString &localfile) {
    QIcon qicon;
    QImage image(localfile);
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
    QPixmap pixmap = qicon.pixmap(QSize(SCREENSHOT_WIDTH,SCREENSHOT_WIDTH), QIcon::Normal, QIcon::On);
    screenshotMap[QUrl(localfile).fileName()]->setPixmap(pixmap);
    screenshotMap[QUrl(localfile).fileName()]->setFixedSize(QSize(SCREENSHOT_WIDTH,SCREENSHOT_WIDTH));
}

void PackageDetails::imageDownloaded(const QString & localfile){
    QIcon qicon;
    QImage image(localfile);
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
    QPixmap pixmap = qicon.pixmap(QSize(ICON_WIDTH,ICON_WIDTH), QIcon::Normal, QIcon::On);
    iconButton->setPixmap(pixmap);
    iconButton->setFixedSize(QSize(ICON_WIDTH,ICON_WIDTH));
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
