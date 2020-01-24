//
// Created by hamzeh on 11/23/19.
//

#include "PackageDetails.h"


PackageDetails::PackageDetails(Package *package, const QString &title, PxQScrollArea *parent) : PxQScrollArea(
        title, parent) {
    auto packageComponent = new PackageComponent(package,this);

    auto pal = QGuiApplication::palette();
    bgColor = pal.color(QPalette::Active, QPalette::Base);
    fgColor = pal.color(QPalette::Active, QPalette::Text);

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

    QWidget *widget=new QWidget(this);
    widget->setLayout(layout);
    setWidgetResizable(true);
    setWidget(widget);
}

QVBoxLayout *PackageDetails::loadRightSide() {
    QFont titleFont("default", PACKAGE_TITLE_FONT_SIZE,QFont::Bold);
    QFont descriptionFont("default", PACKAGE_DESC_FONT_SIZE);
    // add title, license and desc
    QLabel *titleLabel= new QLabel(this->package->title(),this);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet(QString(QLABEL_STYLE_FROM_COLOR_SCHEME).arg(bgColor.name(), fgColor.name()));
    
    QLabel *descriptionLabel= new QLabel(this->package->description(),this);
    descriptionLabel->setStyleSheet(QString(QLABEL_STYLE_FROM_COLOR_SCHEME).arg(bgColor.name(), fgColor.name()));
    descriptionLabel->setFont(descriptionFont);

    QLabel *screenShotsLabel = new QLabel("Screen Shots",this);
    screenShotsLabel->setStyleSheet(QString(QLABEL_STYLE_FROM_COLOR_SCHEME).arg(bgColor.name(), fgColor.name()));
    screenShotsLabel->setFont(titleFont);

    auto screenshotSeperator = new PxLineSeperator(this);

    auto screenshotList = createScreenshotList(package->screenShots());
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
    textLayout->setSpacing(PACKAGE_DETAILS_RIGHT_PANEL_SPACE);
    textLayout->setMargin(PACKAGE_DETAILS_RIGHT_PANEL_MARGIN);

    return textLayout;
}

QListWidget *PackageDetails::createScreenshotList(const QStringList &list) {
    auto screenshotList = new QListWidget;
    screenshotList->setViewMode(QListWidget::IconMode);
    screenshotList->setIconSize(QSize(PACKAGE_SCREENSHOT_W, PACKAGE_SCREENSHOT_H));
    screenshotList->setResizeMode(QListWidget::Adjust);
    screenshotList->setAutoFillBackground(false);
    screenshotList->setStyleSheet(PACKAGE_SCREENSHOT_STYLESHEET);
    screenshotList->setWrapping(false);
    screenshotList->setFixedHeight(PACKAGE_SCREENSHOT_H);
    connect(screenshotList, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(onScreenshotClicked(QListWidgetItem*)));

    int i=0;
    for(auto const l :list){
        QUrl url(l);
        auto scrItem = new ScreenshotItem(package,i++);
        screenshotMap[url.fileName()]=scrItem;
        const char *homedir = getpwuid(getuid())->pw_dir;
        QString iconFileLocalPath = QString(homedir) + QString(PACKAGE_SCREENSHOTS_CACHE_DIR) + QString(this->package->name()) + QString("/");
        QFile iconFile(iconFileLocalPath+url.fileName());
        if(!iconFile.exists()){
            screenshotDownloader = new FileDownloader(url,
                                                      iconFileLocalPath,
                                                      this);
            connect(screenshotDownloader, SIGNAL (downloaded(const QString &)), this, SLOT (screenshotsDownloaded(const QString &)));
        } else
            screenshotsDownloaded(iconFileLocalPath+url.fileName());
        screenshotList->addItem(scrItem);
    }
    return screenshotList;
}

void PackageDetails::screenshotsDownloaded(const QString &localfile) {
    screenshotMap[QUrl(localfile).fileName()]->loadImage(localfile,QSize(PACKAGE_SCREENSHOT_W, PACKAGE_SCREENSHOT_H));
}

void PackageDetails::onScreenshotClicked(QListWidgetItem *item) {
    emit screenshotItemClicked((ScreenshotItem *)item);
}