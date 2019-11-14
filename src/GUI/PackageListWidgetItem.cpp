//
// Created by hamzeh on 11/3/19.
//

#include "PackageListWidgetItem.h"
#define IMAGE_CACHE_DIR "/.cache/px/px-software/images/"
#define BUTTON_WIDTH 128
#define ICON_WIDTH 128

PackageListWidgetItem::PackageListWidgetItem(Package *package, bool removeEnable,QWidget *parent) : QWidget(parent) {
    m_pkgMgr = PackageManager::Instance();
//    m_pkgMgrTrk = PackageManagerTracker::Instance();
    this->removeButtonEnable = removeEnable;
    this->package = package;
    iconRemoteUrl = QUrl(package->icon());
    loadIcon();
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(loadIcon());
    layout->addLayout(loadTexts());
    layout->addLayout(loadButtons());
    this->setLayout(layout);
}

QVBoxLayout *PackageListWidgetItem::loadTexts() {
    QFont titleFont("default", 12,QFont::Bold);
    QFont descriptionFont("default", 10);
    // add title, license and desc
    QLabel *titleLabel= new QLabel(this->package->title());
    titleLabel->setFont(titleFont);

    QLabel *licenseLabel= new QLabel(this->package->version() + " - " + this->package->license());
    licenseLabel->setStyleSheet("QLabel { color : gray; }");

    QLabel *descriptionLabel= new QLabel(this->package->description().mid(0,300).append(" ... more"));
    descriptionLabel->setFont(descriptionFont);
    descriptionLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    descriptionLabel->setWordWrap(true);

    QHBoxLayout *descriptionLayout = new QHBoxLayout;
    descriptionLayout->addWidget(descriptionLabel);

    QHBoxLayout *up = new QHBoxLayout;
    up->addWidget(titleLabel);
    up->addWidget(licenseLabel);

    QHBoxLayout *down = new QHBoxLayout;
    down->addLayout(descriptionLayout);

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->addLayout(up);
    textLayout->addLayout(down);
    return textLayout;
}

QHBoxLayout *PackageListWidgetItem::loadButtons() {
    // add install,update and remove buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    updateButton = new QPushButton;
    updateButton->setText("Update");
    updateButton->setFixedWidth(BUTTON_WIDTH);
    connect(updateButton, SIGNAL(released()), this, SLOT(updateButtonHandler()));
    buttonLayout->addWidget(updateButton);

    removeButton = new QPushButton;
    removeButton->setText("Remove");
    removeButton->setFixedWidth(BUTTON_WIDTH);
    connect(removeButton, SIGNAL(released()), this, SLOT(removeButtonHandler()));
    buttonLayout->addWidget(removeButton);

    installButton = new QPushButton;
    installButton->setText("Install");
    installButton->setFixedWidth(BUTTON_WIDTH);
    connect(installButton, SIGNAL(released()), this, SLOT(installButtonHandler()));
    buttonLayout->addWidget(installButton);
    
    upToDateButton = new QPushButton;
    upToDateButton->setText("Up-To-Date");
    upToDateButton->setFixedWidth(BUTTON_WIDTH);
    buttonLayout->addWidget(upToDateButton);
    reloadButtonsStatus();
    buttonLayout->setAlignment(Qt::AlignRight | Qt::AlignCenter);
    return buttonLayout;
}

void PackageListWidgetItem::reloadButtonsStatus() {
    QString installButtonStyle="QPushButton {background-color: green; color: white;}";
    QString removeButtonStyle="QPushButton {background-color: red; color: white;}";
    QString updateButtonStyle="QPushButton {background-color: blue; color: white;}";
    QString upToDateButtonStyle="QPushButton {background-color: gray; color: black;}";
    updateButton->setVisible(false);
    removeButton->setVisible(false);
    upToDateButton->setVisible(false);
    installButton->setVisible(false);

    if(package->isInstalled()) { // if installed
        if (package->isUpdateAvailable()) { // if upgradable
            updateButton->setText("Update");
            updateButton->setStyleSheet(updateButtonStyle);
            updateButton->setVisible(true);
        }
        if(removeButtonEnable){
            removeButton->setText("Remove");
            removeButton->setStyleSheet(removeButtonStyle);
            removeButton->setVisible(true);
        }
        if(!removeButtonEnable && !(package->isUpdateAvailable())) {
            upToDateButton->setText("Up-To-Date");
            upToDateButton->setStyleSheet(upToDateButtonStyle);
            upToDateButton->setVisible(true);
        }
    } else {
        installButton->setText("Install");
        installButton->setStyleSheet(installButtonStyle);
        installButton->setVisible(true);
    }
}

QHBoxLayout * PackageListWidgetItem::loadIcon() {
    QHBoxLayout *iconLayout = new QHBoxLayout;
    const char *homedir = getpwuid(getuid())->pw_dir;
    QString iconFileLocalPath = QString(homedir)+QString(IMAGE_CACHE_DIR)+QString(this->package->name())+QString("/");
    QFile iconFile(iconFileLocalPath+iconRemoteUrl.fileName());
    if(!iconFile.exists()){
        m_pImgCtrl = new FileDownloader(iconRemoteUrl,
                                        iconFileLocalPath,
                                        this);
        connect(m_pImgCtrl, SIGNAL (downloaded()), this, SLOT (imageDownloaded()));
    }
    iconButton = new QLabel;
    QIcon qicon;
    QImage image(iconFileLocalPath+iconRemoteUrl.fileName());
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
    QPixmap pixmap = qicon.pixmap(QSize(ICON_WIDTH,ICON_WIDTH), QIcon::Normal, QIcon::On);
    iconButton->setPixmap(pixmap);
    iconButton->setFixedSize(QSize(ICON_WIDTH,ICON_WIDTH));
    iconButton->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    iconLayout->addWidget(iconButton);
    return iconLayout;
}

void PackageListWidgetItem::imageDownloaded(){
    QIcon qicon;
    QImage image(m_pImgCtrl->localFilePath.toString());
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
    QPixmap pixmap = qicon.pixmap(QSize(ICON_WIDTH,ICON_WIDTH), QIcon::Normal, QIcon::On);
    iconButton->setPixmap(pixmap);
    iconButton->setFixedSize(QSize(ICON_WIDTH,ICON_WIDTH));
}

void PackageListWidgetItem::reloadPackage() {
    PackageManager *m_pkgMngr = PackageManager::Instance();
    packageReadyConnection = connect(m_pkgMngr, SIGNAL(packageDetailsReady(const QUuid &, Package *)),this, SLOT(packageDetailReadyHandler(const QUuid &, Package *)));
    m_pkgMngr->requestPackageDetails(package->name());
}

void PackageListWidgetItem::installButtonHandler() {
    PackageManagerTracker *m_pkgMgrTrk = PackageManagerTracker::Instance();
    packageProgressConnection = connect(m_pkgMgrTrk, SIGNAL(packageInstalled(const QString)),this, SLOT(packageProgressDoneHandler(const QString)));
    failedProgressConnection = connect(m_pkgMgrTrk, SIGNAL(progressFailed(const QString&,const QString&)),this, SLOT(taskFailedHandler(const QString,const QString&)));
    m_pkgMgrTrk->requestPackageInstallation(package->name());
    installButton->setText("Installing ...");
}

void PackageListWidgetItem::removeButtonHandler() {
    PackageManagerTracker *m_pkgMgrTrk = PackageManagerTracker::Instance();
    packageProgressConnection = connect(m_pkgMgrTrk, SIGNAL(packageRemoved(const QString)),this, SLOT(packageProgressDoneHandler(const QString)));
    failedProgressConnection = connect(m_pkgMgrTrk, SIGNAL(progressFailed(const QString&,const QString&)),this, SLOT(taskFailedHandler(const QString,const QString&)));
    m_pkgMgrTrk->requestPackageRemoval(package->name());
    removeButton->setText("Removing ...");
}

void PackageListWidgetItem::updateButtonHandler() {
    PackageManagerTracker *m_pkgMgrTrk = PackageManagerTracker::Instance();
    packageProgressConnection = connect(m_pkgMgrTrk, SIGNAL(packageUpdated(const QString)),this, SLOT(packageProgressDoneHandler(const QString)));
    failedProgressConnection = connect(m_pkgMgrTrk, SIGNAL(progressFailed(const QString&,const QString&)),this, SLOT(taskFailedHandler(const QString,const QString&)));
    m_pkgMgrTrk->requestPackageUpdate(package->name());
    updateButton->setText("Updating ...");
}

void PackageListWidgetItem::packageProgressDoneHandler(const QString &name) {
    if(name == package->name()){
        disconnect(packageProgressConnection);
        reloadPackage();
    }
}

void PackageListWidgetItem::taskFailedHandler(const QString &name, const QString &message) {
    if(name == package->name()){
        disconnect(failedProgressConnection);
        reloadButtonsStatus();
    }
}

void PackageListWidgetItem::packageDetailReadyHandler(const QUuid & taskId, Package *package){
    if(this->package->name() == package->name()) {
        disconnect(failedProgressConnection);
        delete this->package;
        this->package=package;
        reloadButtonsStatus();
    }
}