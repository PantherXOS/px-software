//
// Created by hamzeh on 11/3/19.
//

#include "PackageListWidgetItem.h"
#define IMAGE_CACHE_DIR "/.cache/px/px-software/images/"
#define BUTTON_WIDTH 128
#define BUTTON_HEIGHT 28
#define ICON_WIDTH 128

PackageListWidgetItem::PackageListWidgetItem(Package *package, bool removeEnable ,QWidget *parent) : QWidget(parent) {
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    connect(m_pkgMgrTrk, SIGNAL(taskDataReceived(const QString&,const QString&)),this, SLOT(taskDataReceivedHandler(const QString,const QString&)));
    connect(m_pkgMgrTrk, SIGNAL(packageUpdated(const QString)),this, SLOT(packageUpdatedHandler(const QString)));
    connect(m_pkgMgrTrk, SIGNAL(packageRemoved(const QString)),this, SLOT(packageRemovedHandler(const QString)));
    connect(m_pkgMgrTrk, SIGNAL(packageInstalled(const QString)),this, SLOT(packageInstalledHandler(const QString)));
    connect(m_pkgMgrTrk, SIGNAL(packageTaskCanceled(const QString)),this, SLOT(taskCanceledHandler(const QString)));
    failedProgressConnection  = connect(m_pkgMgrTrk, SIGNAL(progressFailed(const QString&,const QString&)),this, SLOT(taskFailedHandler(const QString,const QString&)));

    this->removeButtonEnable = removeEnable;
    this->package = package;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(loadIcon(QUrl(package->icon())));
    layout->addLayout(loadTexts());
    layout->addLayout(loadButtons());
//    setObjectName("PackageListWidgetItem");
//    setStyleSheet("PackageListWidgetItem {border:1px solid rgb(80, 80, 80);}");
    this->setLayout(layout);
}

QHBoxLayout * PackageListWidgetItem::loadIcon(const QUrl &iconUrl) {
    iconButton = new QLabel(this);
    iconButton->setFixedSize(QSize(ICON_WIDTH,ICON_WIDTH));
    iconButton->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);;
    iconButton->setStyleSheet("QLabel {border 1px solid rgb(80, 80, 80);}");

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

QVBoxLayout *PackageListWidgetItem::loadTexts() {
    QFont titleFont("default", 12,QFont::Bold);
    QFont descriptionFont("default", 10);
    // add title, license and desc
    QLabel *titleLabel= new QLabel(this->package->title(),this);
    titleLabel->setFont(titleFont);

    QLabel *licenseLabel= new QLabel(this->package->version() + " - " + this->package->license(),this);
    licenseLabel->setStyleSheet("QLabel { color : gray; }");

    QLabel *descriptionLabel= new QLabel(this->package->description().mid(0,300).append(" ... more"),this);
    descriptionLabel->setFont(descriptionFont);
    descriptionLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    descriptionLabel->setWordWrap(true);

    auto descriptionLayout = new QHBoxLayout;
    descriptionLayout->addWidget(descriptionLabel);

    auto up = new QHBoxLayout;
    up->addWidget(titleLabel);
    up->addWidget(licenseLabel);

    auto down = new QHBoxLayout;
    down->addLayout(descriptionLayout);

    auto textLayout = new QVBoxLayout;
    textLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    textLayout->addLayout(up);
    textLayout->addLayout(down);
    return textLayout;
}

QHBoxLayout *PackageListWidgetItem::loadButtons() {
    // add install,update and remove buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    updateButton = new QPushButton(this);
    updateButton->setText("Update");
    updateButton->setFixedSize(BUTTON_WIDTH,BUTTON_HEIGHT);
    updateButton->setStyleSheet("QPushButton {background-color: green; color: white;}");
    connect(updateButton, SIGNAL(released()), this, SLOT(updateButtonHandler()));
    buttonLayout->addWidget(updateButton);

    removeButton = new QPushButton(this);
    removeButton->setText("Remove");
    removeButton->setFixedSize(BUTTON_WIDTH,BUTTON_HEIGHT);
    removeButton->setStyleSheet("QPushButton {background-color: red; color: white;}");
    connect(removeButton, SIGNAL(released()), this, SLOT(removeButtonHandler()));
    buttonLayout->addWidget(removeButton);

    installButton = new QPushButton(this);
    installButton->setText("Install");
    installButton->setFixedSize(BUTTON_WIDTH,BUTTON_HEIGHT);
    installButton->setStyleSheet("QPushButton {background-color: blue; color: white;}");
    connect(installButton, SIGNAL(released()), this, SLOT(installButtonHandler()));
    buttonLayout->addWidget(installButton);

    upToDateButton = new QPushButton(this);
    upToDateButton->setText("Up-To-Date");
    upToDateButton->setFixedSize(BUTTON_WIDTH,BUTTON_HEIGHT);
    upToDateButton->setStyleSheet("QPushButton {background-color: gray; color: black;}");
    buttonLayout->addWidget(upToDateButton);

    reloadButtonsStatus();
    buttonLayout->setAlignment(Qt::AlignRight | Qt::AlignCenter);
    return buttonLayout;
}

void PackageListWidgetItem::reloadButtonsStatus() {
    updateButton->setVisible(false);
    removeButton->setVisible(false);
    upToDateButton->setVisible(false);
    installButton->setVisible(false);
    QIcon stopIcon(":images/general/src/GUI/resources/stop");
    QSize stopIconSize(20,20);
    if(m_pkgMgrTrk->inInstalling(package->name())) {
        installButton->setText("Installing ...");
        installButton->setIcon(stopIcon);
        installButton->setIconSize(stopIconSize);
        installButton->setVisible(true);
    } else if(m_pkgMgrTrk->inRemoving(package->name())) {
        removeButton->setText("Removing ...");
        removeButton->setIcon(stopIcon);
        removeButton->setIconSize(stopIconSize);
        removeButton->setVisible(true);
    } else if(m_pkgMgrTrk->inUpdating(package->name())) {
        updateButton->setText("Updating ...");
        updateButton->setIcon(stopIcon);
        updateButton->setIconSize(stopIconSize);
        updateButton->setVisible(true);
    } else {
        if(package->isInstalled()) {
            if (package->isUpdateAvailable()) {
                updateButton->setText("Update");
                updateButton->setIcon(QIcon());
                updateButton->setVisible(true);
            }
            if(removeButtonEnable){
                removeButton->setText("Remove");
                removeButton->setIcon(QIcon());
                removeButton->setVisible(true);
            }
            if(!removeButtonEnable && !(package->isUpdateAvailable())) {
                upToDateButton->setText("Up-To-Date");
                upToDateButton->setIcon(QIcon());
                upToDateButton->setVisible(true);
            }
        } else {
            installButton->setText("Install");
            installButton->setIcon(QIcon());
            installButton->setVisible(true);
        }
    }
}

void PackageListWidgetItem::imageDownloaded(QString localfile){
    QIcon qicon;
    QImage image(localfile);
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
    QPixmap pixmap = qicon.pixmap(QSize(ICON_WIDTH,ICON_WIDTH), QIcon::Normal, QIcon::On);
    iconButton->setPixmap(pixmap);
}

void PackageListWidgetItem::installButtonHandler() {
    if(m_pkgMgrTrk->requestPackageInstallation(package->name())) {
        reloadButtonsStatus();
    } else {
        m_pkgMgrTrk->requestPackageTaskCancel(package->name());
    }
}

void PackageListWidgetItem::removeButtonHandler() {
    if(m_pkgMgrTrk->requestPackageRemoval(package->name())) {
        reloadButtonsStatus();
    } else {
        m_pkgMgrTrk->requestPackageTaskCancel(package->name());
    }
}

void PackageListWidgetItem::updateButtonHandler() {
    if(m_pkgMgrTrk->requestPackageUpdate(package->name())) {
        reloadButtonsStatus();
    } else {
        m_pkgMgrTrk->requestPackageTaskCancel(package->name());
    }
}

void PackageListWidgetItem::packageUpdatedHandler(const QString &name) {
    if(name == package->name()){
        this->package->setUpdateAvailable(false);
        reloadButtonsStatus();
    }
}

void PackageListWidgetItem::packageRemovedHandler(const QString &name) {
    if(name == package->name()){
        this->package->setInstalled(false);
        reloadButtonsStatus();
    }
}

void PackageListWidgetItem::packageInstalledHandler(const QString &name) {
    if(name == package->name()){
        this->package->setInstalled(true);
        reloadButtonsStatus();
    }
}

void PackageListWidgetItem::taskFailedHandler(const QString &name, const QString &message) {
    if(name == package->name()){
        disconnect(failedProgressConnection);
        reloadButtonsStatus();
    }
}

void PackageListWidgetItem::taskDataReceivedHandler(const QString & name, const QString &message) {
    if(this->package->name() == name){
        debugMessage+=message;
        QStringList lines = debugMessage.split('\r');
        debugMessage = lines.at(0);
        if(lines.size()>2)
            debugMessage+=lines.at(lines.size()-1);
        if(this->terminal != nullptr)
            this->terminal->showMessage(debugMessage);
    }
}

void PackageListWidgetItem::taskCanceledHandler(const QString &name) {
    if(name == package->name()) {
        reloadButtonsStatus();
    }
}

Package * &PackageListWidgetItem::getPackage() {
    return this->package;
}

TerminalWidget *PackageListWidgetItem::getTerminal() {
    if(this->terminal == nullptr)
        this->terminal = new TerminalWidget(0,package->name());
    this->terminal->showMessage(debugMessage);
    return this->terminal;
}
