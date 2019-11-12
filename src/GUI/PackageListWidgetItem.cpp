//
// Created by hamzeh on 11/3/19.
//

#include "PackageListWidgetItem.h"
#define IMAGE_CACHE_DIR "/.cache/px/px-software/images/"
#define BUTTON_WIDTH 128
#define ICON_WIDTH 128

PackageListWidgetItem::PackageListWidgetItem(PKG::Package *package, bool removeEnable,QWidget *parent) : QWidget(parent) {
    m_pkgMgr = PKG::PackageManager::Instance();
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
    installButton->setVisible(false);

    if(package->isInstalled()) { // if installed
        if (package->isUpdateAvailable()) { // if upgradable
            updateButton->setStyleSheet(updateButtonStyle);
            updateButton->setVisible(true);
        }
        if(removeButtonEnable){
            removeButton->setStyleSheet(removeButtonStyle);
            removeButton->setVisible(true);
        }
        if(!removeButtonEnable && !(package->isUpdateAvailable())) {
            installButton->setText("Up-To-Date");
            installButton->setStyleSheet(upToDateButtonStyle);
            installButton->setVisible(true);
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
    QString m_dbPath = "./SAMPLE_DB";
    PKG::DataAccessLayer *dbLayer = new PKG::DataAccessLayer(m_dbPath);
    auto pkg = dbLayer->packageDetails(package->name());
    delete package;
    package=pkg;
}

void PackageListWidgetItem::installButtonHandler() {
    installationSignalConnection = connect(m_pkgMgr, SIGNAL(packageInstalled(const QString)),this, SLOT(packagedInstalledHandler(const QString)));
    failedTaskSignalConnection = connect(m_pkgMgr, SIGNAL(taskFailed(const QUuid &, const QString &)),this, SLOT(taskFailedHandler(const QUuid &, const QString &)));
    dataReceivedConnection = connect(m_pkgMgr, SIGNAL(newTaskData(const QUuid &, const QString &)), this, SLOT(taskDataHandler(const QUuid &, const QString &)));
    taskDoneSignalConnection = connect(m_pkgMgr, SIGNAL(taskDone(const QUuid &, const QString &)), this, SLOT(taskDoneHandler(const QUuid &, const QString &)));
    installButton->setText("Installing ...");
//    m_pkgMgr->requestPackageInstallation(package->name());
    PackageManagerTracker::Instance().requestPackageInstallation(package->name());
}

void PackageListWidgetItem::removeButtonHandler() {
    cout << " TBD - removeButtonHandler" << endl;
    removeSignalConnection = connect(m_pkgMgr, SIGNAL(packageRemoved(const QString)),this, SLOT(packagedRemovedHandler(const QString)));
    failedTaskSignalConnection = connect(m_pkgMgr, SIGNAL(taskFailed(const QUuid &, const QString &)),this, SLOT(taskFailedHandler(const QUuid &, const QString &)));
    dataReceivedConnection = connect(m_pkgMgr, SIGNAL(newTaskData(const QUuid &, const QString &)), this, SLOT(taskDataHandler(const QUuid &, const QString &)));
    taskDoneSignalConnection = connect(m_pkgMgr, SIGNAL(taskDone(const QUuid &, const QString &)), this, SLOT(taskDoneHandler(const QUuid &, const QString &)));
    removeButton->setText("Removing ...");
//    m_pkgMgr->requestPackageRemoval(package->name());
    PackageManagerTracker::Instance().requestPackageRemoval(package->name());
}

void PackageListWidgetItem::updateButtonHandler() {
    cout << " TBD - updateButtonHandler" << endl;
    updateSignalConnection = connect(m_pkgMgr, SIGNAL(packageUpdated(const QStringList)),this, SLOT(packagedUpdatedHandler(const QStringList)));
    failedTaskSignalConnection = connect(m_pkgMgr, SIGNAL(taskFailed(const QUuid &, const QString &)),this, SLOT(taskFailedHandler(const QUuid &, const QString &)));
    dataReceivedConnection = connect(m_pkgMgr, SIGNAL(newTaskData(const QUuid &, const QString &)), this, SLOT(taskDataHandler(const QUuid &, const QString &)));
    taskDoneSignalConnection = connect(m_pkgMgr, SIGNAL(taskDone(const QUuid &, const QString &)), this, SLOT(taskDoneHandler(const QUuid &, const QString &)));
    updateButton->setText("Updating ...");
    QStringList packages = {package->name()};
//    m_pkgMgr->requestPackageUpdate(packages);
    PackageManagerTracker::Instance().requestPackageUpdate(packages);
}

void PackageListWidgetItem::packagedUpdatedHandler(const QStringList &nameList) {
    qDebug() << nameList  << " DBG - package updated";
    disconnect(updateSignalConnection);
    disconnect(failedTaskSignalConnection);
    disconnect(dataReceivedConnection);
    reloadPackage();
    reloadButtonsStatus();
}

void PackageListWidgetItem::packagedRemovedHandler(const QString &name) {
    qDebug() << name  << " DBG - package removed";
    disconnect(removeSignalConnection);
    disconnect(failedTaskSignalConnection);
    disconnect(dataReceivedConnection);
    reloadPackage();
    reloadButtonsStatus();
}

void PackageListWidgetItem::packagedInstalledHandler(const QString &name){
    qDebug() << name  << " DBG - package installed";
    disconnect(installationSignalConnection);
    disconnect(failedTaskSignalConnection);
    disconnect(dataReceivedConnection);
    reloadPackage();
    reloadButtonsStatus();
}

void PackageListWidgetItem::taskFailedHandler(const QUuid &uuid, const QString &message){
    qDebug() << uuid << " " << message;
    disconnect(installationSignalConnection);
    disconnect(updateSignalConnection);
    disconnect(removeSignalConnection);
    disconnect(failedTaskSignalConnection);
    disconnect(dataReceivedConnection);
    disconnect(taskDoneSignalConnection);
    reloadButtonsStatus();
}

void PackageListWidgetItem::taskDoneHandler(const QUuid &uuid, const QString &message){
    qDebug() << uuid << " " << message;
    disconnect(failedTaskSignalConnection);
    disconnect(dataReceivedConnection);
    disconnect(taskDoneSignalConnection);
    reloadButtonsStatus();
}

void PackageListWidgetItem::taskDataHandler(const QUuid &taskId, const QString &data){
    qDebug() << taskId << data;
}
