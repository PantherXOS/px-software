//
// Created by hamzeh on 10/22/19.
//

#include "CategoryWidget.h"
#define IMAGE_CACHE_DIR "/.cache/px/px-software/categories/"
#define ICON_WIDTH 64

CategoryWidget::CategoryWidget(Category *category,QWidget *parent) : QWidget(parent) {
    QFont titleFont("default", 12,QFont::Bold);
    QFont descriptionFont("default", 10);

    name = category->name();
    title = category->title();
    description = category->description();
    icon = category->icon();

    loadIcon();

    QLabel *titleLabel= new QLabel();
    titleLabel->setText(title);
    titleLabel->setFont(titleFont);

    QLabel *descriptionLabel = new QLabel();
    descriptionLabel->setText(description);
    descriptionLabel->setFont(descriptionFont);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(titleLabel);
    vLayout->addWidget(descriptionLabel);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(iconButton);
    layout->addLayout(vLayout);

    setLayout(layout);
}

PxQScrollArea * CategoryWidget::getPackageList() {
    PackageManager *m_pkgMgr = PackageManager::Instance();
    connect(m_pkgMgr, SIGNAL(taskFailed(const QUuid &, const QString &)),this, SLOT(taskFailedHandler(const QUuid &, const QString &)));
    connect(m_pkgMgr, SIGNAL(categoryPackagesReady(const QUuid &,const QVector<Package *> &)),this, SLOT(categoryPackagesReadyHandler(const QUuid &,const QVector<Package *> &)));
    m_pkgMgr->requestCategoryPackages(name);
    QVector<Package *> packages;
    packageListWidget = new PackageListWidget(packages,false,0,name);
    return packageListWidget;
}

void CategoryWidget::loadIcon() {
    iconButton = new QLabel;
    // check url is weblink or name of resource file
    const char *homedir = getpwuid(getuid())->pw_dir;
    QRegExp rx("https?:\\/\\/(?:www\\.|(?!www))[a-zA-Z0-9][a-zA-Z0-9-]+[a-zA-Z0-9]\\.[^\\s]{2,}|www\\.[a-zA-Z0-9][a-zA-Z0-9-]+[a-zA-Z0-9]\\.[^\\s]{2,}|https?:\\/\\/(?:www\\.|(?!www))[a-zA-Z0-9]+\\.[^\\s]{2,}|www\\.[a-zA-Z0-9]+\\.[^\\s]{2,}");
    if(rx.exactMatch(icon)) {
        QString iconFileLocalPath = QString(homedir)+QString(IMAGE_CACHE_DIR)+QString(name)+QString("/");
        QFile iconFile(iconFileLocalPath+QUrl(icon).fileName());
        if(!iconFile.exists()) {
            m_pImgCtrl = new FileDownloader(icon,
                                            iconFileLocalPath,
                                            this);
            connect(m_pImgCtrl, SIGNAL (downloaded()), this, SLOT (imageDownloaded()));
        }
        icon = iconFileLocalPath+QUrl(icon).fileName();
    } else {
        icon =  QString(":/category/icons/") + icon;
    }
    QIcon qicon;
    QImage image(icon);
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
    QPixmap pixmap = qicon.pixmap(QSize(ICON_WIDTH,ICON_WIDTH), QIcon::Normal, QIcon::On);
    iconButton->setPixmap(pixmap);
    iconButton->setFixedSize(QSize(ICON_WIDTH,ICON_WIDTH));
}

void CategoryWidget::imageDownloaded(){
    QIcon qicon;
    QImage image(m_pImgCtrl->localFilePath.toString());
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
    QPixmap pixmap = qicon.pixmap(QSize(ICON_WIDTH,ICON_WIDTH), QIcon::Normal, QIcon::On);
    iconButton->setPixmap(pixmap);
    iconButton->setFixedSize(QSize(ICON_WIDTH,ICON_WIDTH));
}

void CategoryWidget::categoryPackagesReadyHandler(const QUuid &taskId,const QVector<Package *> & packages){
    packageListWidget->update(packages);
}

void CategoryWidget::taskFailedHandler(const QUuid &taskId, const QString & message) {
    qDebug() << this << " : "<< message;
}
