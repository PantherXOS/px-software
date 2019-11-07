//
// Created by hamzeh on 11/3/19.
//

#include "PackageWidget.h"
#define IMAGE_CACHE_DIR "/.cache/px/px-software/images/"
#define BUTTON_WIDTH 128
#define ICON_WIDTH 128

PackageWidget::PackageWidget(PKG::Package *package, bool installEnable, bool updateEnable, bool removeEnable) {
    showMaximized();
    this->package = package;
    iconRemoteUrl = QUrl(package->icon());
    loadIcon();
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(loadIcon());
    layout->addLayout(loadTexts());
    layout->addLayout(loadButtons(installEnable, updateEnable, removeEnable));
    this->setLayout(layout);
}

QVBoxLayout *PackageWidget::loadTexts() {
    QFont titleFont("default", 12,QFont::Bold);
    QFont descriptionFont("default", 10);
    // add title, license and desc
    QLabel *titleLabel= new QLabel(this->package->title());
    titleLabel->setFont(titleFont);
    titleLabel->showMaximized();

    QLabel *licenseLabel= new QLabel(this->package->license());
    licenseLabel->showMaximized();
    licenseLabel->setStyleSheet("QLabel { color : gray; }");

    QLabel *descriptionLabel= new QLabel(this->package->description().mid(0,300).append(" ... more"));
    descriptionLabel->setFont(descriptionFont);
    descriptionLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->showMaximized();

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

QHBoxLayout *PackageWidget::loadButtons(bool installEnable, bool updateEnable, bool removeEnable){
    QString installButtonStyle="QPushButton {background-color: green; color: white;}";
    QString removeButtonStyle="QPushButton {background-color: red; color: white;}";
    QString updateButtonStyle="QPushButton {background-color: blue; color: white;}";

    // add install,update and remove buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    if(package->isInstalled()) { // if installed
        if (package->isUpdateAvailable() & updateEnable) { // if updatable
            updateButton = new QPushButton;
            updateButton->setText("Update");
            updateButton->setFixedWidth(BUTTON_WIDTH);
            updateButton->setStyleSheet(updateButtonStyle);
            connect(updateButton, SIGNAL(released()), this, SLOT(updateButtonHandler()));
            buttonLayout->addWidget(updateButton);
        } else if(removeEnable){
            removeButton = new QPushButton;
            removeButton->setText("Remove");
            removeButton->setFixedWidth(BUTTON_WIDTH);
            removeButton->setStyleSheet(removeButtonStyle);
            connect(removeButton, SIGNAL(released()), this, SLOT(removeButtonHandler()));
            buttonLayout->addWidget(removeButton);
        }
    } else if (installEnable){
        installButton = new QPushButton;
        installButton->setText("Install");
        installButton->setFixedWidth(BUTTON_WIDTH);
        installButton->setStyleSheet(installButtonStyle);
        connect(installButton, SIGNAL(released()), this, SLOT(installButtonHandler()));
        buttonLayout->addWidget(installButton);
    }
    buttonLayout->setAlignment(Qt::AlignRight | Qt::AlignCenter);
    return buttonLayout;
}

QHBoxLayout * PackageWidget::loadIcon() {
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
