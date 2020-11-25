/*
 * px-software - Qt-GUI package manager for guix on PantherX OS
 * Copyright (C) 2019, 2020  Hamzeh Nasajpour <h.nasajpour@pantherx.org>
 * Copyright (C) 2019, 2020  Reza Alizadeh Majd <r.majd@pantherx.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "CategoryWidget.h"

CategoryWidget::CategoryWidget(Category *category,PXWidget *parent) : PXWidget(parent) {
    QFont titleFont("default", CATEGORY_TITLE_FONT_SIZE,QFont::Bold);

    this->category = category;

    loadIcon();

    QLabel *titleLabel= new QLabel(this);
    titleLabel->setText(category->title());
    titleLabel->setFont(titleFont);
    titleLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    titleLabel->setStyleSheet(PACKAGE_LIST_LABELS_STYLESHEET);
    titleLabel->setMargin(5);

    auto titleLayout = new QVBoxLayout;
    titleLayout->addWidget(titleLabel);
    titleLayout->setAlignment(Qt::AlignLeft | Qt::AlignCenter);

    auto seperator = new PXSeperator(this);
    auto seperatorLayout = new QVBoxLayout;
    seperatorLayout->addWidget(seperator);
    seperatorLayout->setAlignment(Qt::AlignBottom);

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addWidget(iconButton);
    hlayout->addLayout(titleLayout);
    hlayout->setMargin(5);
    
    auto layout = new QVBoxLayout;
    layout->addLayout(hlayout);
    layout->addLayout(seperatorLayout);
    layout->setAlignment(Qt::AlignCenter);
    layout->setMargin(0);
    layout->setSpacing(0);
    
    setLayout(layout);
    setToolTip(category->description());
    setFixedSize(CATEGORY_ITEM_WIDTH,CATEGORY_ITEM_HEIGHT);
    auto pal = QGuiApplication::palette();
    auto bgcolor =  pal.color(QPalette::Normal, QPalette::Highlight);
    setStyleSheet(QString::fromLatin1(ITEM_HOVER_STYLESHEET).arg(bgcolor.name()));
}

Category * CategoryWidget::getCategory() {
    return category;
}

void CategoryWidget::loadIcon() {
    iconButton = new QLabel(this);
    iconButton->setAlignment(Qt::AlignCenter);
    iconButton->setStyleSheet(PACKAGE_LIST_LABELS_STYLESHEET);
    // check url is weblink or name of resource file
    QRegExp rx("https?:\\/\\/(?:www\\.|(?!www))[a-zA-Z0-9][a-zA-Z0-9-]+[a-zA-Z0-9]\\.[^\\s]{2,}|www\\.[a-zA-Z0-9][a-zA-Z0-9-]+[a-zA-Z0-9]\\.[^\\s]{2,}|https?:\\/\\/(?:www\\.|(?!www))[a-zA-Z0-9]+\\.[^\\s]{2,}|www\\.[a-zA-Z0-9]+\\.[^\\s]{2,}");
    if(rx.exactMatch(category->icon())) {
        QString iconFileLocalPath = CacheManager::instance()->cacheDir()+CATEGORY_ICON_CACHE_DIR+QString(category->name())+QString("/");
        icon = iconFileLocalPath+QUrl(category->icon()).fileName();
        m_pImgCtrl = new FileDownloader(this);
        connect(m_pImgCtrl, SIGNAL (downloaded(const QString &)), this, SLOT (imageDownloaded(const QString &)));
        m_pImgCtrl->start(category->icon(), iconFileLocalPath);
    } else {
        icon = category->icon();
        imageDownloaded(icon);
    }
}

void CategoryWidget::imageDownloaded(const QString & localfile){
    QIcon qicon;
    QImage image(":images/general/src/GUI/resources/def_category");
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);

    QIcon _icon(QIcon::fromTheme(localfile,qicon));
    QPixmap pixmap = _icon.pixmap(QSize(CATEGORY_ICON_SIZE, CATEGORY_ICON_SIZE), QIcon::Normal, QIcon::On);
    iconButton->setPixmap(pixmap);
    iconButton->setFixedSize(QSize(CATEGORY_ICON_SIZE, CATEGORY_ICON_SIZE));
}

void CategoryWidget::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}