//
// Created by hamzeh on 11/3/19.
//

#include "PackageWidget.h"
#define IMAGE_CACHE_DIR "/.cache/px/px-software/images/"
#define BUTTON_WIDTH 128
#define ICON_WIDTH 128

PackageWidget::PackageWidget(PKG::Package *package) {
    QFont titleFont("default", 12,QFont::Bold);
    QFont descriptionFont("default", 10);
    QString installButtonStyle="QPushButton {background-color: green; color: white;}";
    QString removeButtonStyle="QPushButton {background-color: red; color: white;}";
    QString updateButtonStyle="QPushButton {background-color: blue; color: white;}";
    name = package->name();
    title = package->title();
    version = package->version();
    description = package->description();
    homepage = package->homePage();
    license = package->license();
    iconRemoteUrl = QUrl(package->icon());
    screenshotsList = package->screenShots();
    categoriesList = package->categories();

    loadIcon();

    // add title, license and desc
    QLabel *titleLabel= new QLabel(title);
    titleLabel->setFont(titleFont);
    titleLabel->showMaximized();
    QLabel *licenseLabel= new QLabel(license);
    licenseLabel->showMaximized();
    licenseLabel->setStyleSheet("QLabel { color : gray; }");
    QLabel *descriptionLabel= new QLabel(description.mid(0,300).append(" ... more"));
    descriptionLabel->setFont(descriptionFont);
    descriptionLabel->showMaximized();
    QHBoxLayout *up = new QHBoxLayout;
    up->addWidget(titleLabel);
    up->addWidget(licenseLabel);

    QHBoxLayout *down = new QHBoxLayout;
    down->addWidget(descriptionLabel);

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->addLayout(up);
    textLayout->addLayout(down);

    // add install,update and remove buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    if(package->isInstalled()) { // if installed
        if (package->isUpdateAvailable()) { // if updatable
            QPushButton *updateButton = new QPushButton;
            updateButton->setText("Update");
            updateButton->setFixedWidth(BUTTON_WIDTH);
            updateButton->setStyleSheet(updateButtonStyle);
            connect(updateButton, SIGNAL(released()), this, SLOT(updateButtonHandler()));
            buttonLayout->addWidget(updateButton);
        }
        QPushButton *removeButton = new QPushButton;
        removeButton->setText("Remove");
        removeButton->setFixedWidth(BUTTON_WIDTH);
        removeButton->setStyleSheet(removeButtonStyle);
        connect(removeButton, SIGNAL(released()), this, SLOT(removeButtonHandler()));
        buttonLayout->addWidget(removeButton);
    } else {
        QPushButton *installButton = new QPushButton;
        installButton->setText("Install");
        installButton->setFixedWidth(BUTTON_WIDTH);
        installButton->setStyleSheet(installButtonStyle);
        connect(installButton, SIGNAL(released()), this, SLOT(installButtonHandler()));
        buttonLayout->addWidget(installButton);
    }
    buttonLayout->setAlignment(Qt::AlignRight | Qt::AlignCenter);

    QHBoxLayout *layout = new QHBoxLayout;
    //    QHBoxLayout *iconLayout = new QHBoxLayout;
    //    iconLayout->addWidget(iconButton);
    //    iconLayout->setAlignment(Qt::AlignRight | Qt::AlignCenter);
    //    layout->addLayout(iconLayout);
    layout->addWidget(iconButton);
    layout->addLayout(textLayout);
    layout->addLayout(buttonLayout);
    this->setLayout(layout);
}

void PackageWidget::loadIcon() {
    const char *homedir = getpwuid(getuid())->pw_dir;
    QString iconFileLocalPath = QString(homedir)+QString(IMAGE_CACHE_DIR)+QString(name)+QString("/");
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
}

void PackageWidget::imageDownloaded(){
    QIcon qicon;
    QImage image(m_pImgCtrl->localFilePath.toString());
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
    QPixmap pixmap = qicon.pixmap(QSize(ICON_WIDTH,ICON_WIDTH), QIcon::Normal, QIcon::On);
    iconButton->setPixmap(pixmap);
    iconButton->setFixedSize(QSize(ICON_WIDTH,ICON_WIDTH));
}

void PackageWidget::installButtonHandler() {
    cout << " TBD - installButtonHandler" << endl;
}

void PackageWidget::removeButtonHandler() {
    cout << " TBD - removeButtonHandler" << endl;
}

void PackageWidget::updateButtonHandler() {
    cout << " TBD - updateButtonHandler" << endl;
}
