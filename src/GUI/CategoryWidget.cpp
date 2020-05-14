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

CategoryWidget::CategoryWidget(Category *category,QWidget *parent) : QWidget(parent) {
    QFont titleFont("default", CATEGORY_TITLE_FONT_SIZE,QFont::Bold);

    this->category = category;

    loadIcon();

    QLabel *titleLabel= new QLabel(this);
    titleLabel->setText(category->title());
    titleLabel->setFont(titleFont);
    titleLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    auto titleLayout = new QVBoxLayout;
    titleLayout->addWidget(titleLabel);
    titleLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    auto seperator = new PxLineSeperator(this);
    auto seperatorLayout = new QVBoxLayout;
    seperatorLayout->addWidget(seperator);
    titleLayout->setAlignment(Qt::AlignBottom);

    auto vLayout = new QVBoxLayout;
    vLayout->addLayout(titleLayout);
    vLayout->addLayout(seperatorLayout);
    vLayout->setAlignment(Qt::AlignCenter);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(iconButton);
    layout->addLayout(vLayout);

    setLayout(layout);
    setToolTip(category->description());
    setFixedSize(CATEGORY_ITEM_WIDTH,CATEGORY_ITEM_HEIGHT);
}

Category * CategoryWidget::getCategory() {
    return category;
}

void CategoryWidget::loadIcon() {
    iconButton = new QLabel(this);
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
