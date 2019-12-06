//
// Created by hamzeh on 10/22/19.
//

#include "CategoryWidget.h"
#define CATEGORY_ICON_CACHE_DIR "/.cache/px/px-software/categories/"
#define CATEGORY_ICON_W 64

CategoryWidget::CategoryWidget(Category *category,QWidget *parent) : QWidget(parent) {
    QFont titleFont("default", 12,QFont::Bold);
    QFont descriptionFont("default", 10);

    this->category = category;

    loadIcon();

    QLabel *titleLabel= new QLabel(this);
    titleLabel->setText(category->title());
    titleLabel->setFont(titleFont);

    QLabel *descriptionLabel = new QLabel(this);
    descriptionLabel->setText(category->description());
    descriptionLabel->setFont(descriptionFont);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    vLayout->addWidget(titleLabel);
    vLayout->addWidget(descriptionLabel);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(iconButton);
    layout->addLayout(vLayout);

    setLayout(layout);
}

Category * CategoryWidget::getCategory() {
    return category;
}

void CategoryWidget::loadIcon() {
    iconButton = new QLabel(this);
    // check url is weblink or name of resource file
    const char *homedir = getpwuid(getuid())->pw_dir;
    QRegExp rx("https?:\\/\\/(?:www\\.|(?!www))[a-zA-Z0-9][a-zA-Z0-9-]+[a-zA-Z0-9]\\.[^\\s]{2,}|www\\.[a-zA-Z0-9][a-zA-Z0-9-]+[a-zA-Z0-9]\\.[^\\s]{2,}|https?:\\/\\/(?:www\\.|(?!www))[a-zA-Z0-9]+\\.[^\\s]{2,}|www\\.[a-zA-Z0-9]+\\.[^\\s]{2,}");
    if(rx.exactMatch(category->icon())) {
        QString iconFileLocalPath = QString(homedir)+QString(CATEGORY_ICON_CACHE_DIR)+QString(category->name())+QString("/");
        QFile iconFile(iconFileLocalPath+QUrl(category->icon()).fileName());
        if(!iconFile.exists()) {
            m_pImgCtrl = new FileDownloader(category->icon(),
                                            iconFileLocalPath,
                                            this);
            connect(m_pImgCtrl, SIGNAL (downloaded(const QString &)), this, SLOT (imageDownloaded()));
        }
        icon = iconFileLocalPath+QUrl(category->icon()).fileName();
    } else {
        icon =  QString(":/category/icons/") + category->icon();
    }
    QIcon qicon;
    QImage image(icon);
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
    QPixmap pixmap = qicon.pixmap(QSize(CATEGORY_ICON_W,CATEGORY_ICON_W), QIcon::Normal, QIcon::On);
    iconButton->setPixmap(pixmap);
    iconButton->setFixedSize(QSize(CATEGORY_ICON_W,CATEGORY_ICON_W));
}

void CategoryWidget::imageDownloaded(const QString & localfile){
    QIcon qicon;
    QImage image(localfile);
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
    QPixmap pixmap = qicon.pixmap(QSize(CATEGORY_ICON_W,CATEGORY_ICON_W), QIcon::Normal, QIcon::On);
    iconButton->setPixmap(pixmap);
    iconButton->setFixedSize(QSize(CATEGORY_ICON_W,CATEGORY_ICON_W));
}
