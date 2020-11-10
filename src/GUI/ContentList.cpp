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

#include "ContentList.h"

ContentList::ContentList(QListWidget *parent) : QListWidget(parent) {
    //-----------------------------------------------------------------
    contentTitleMap[STORE_LATEST] = tr("Latest");
    contentIDMap[STORE_LATEST] = tr("Latest");
    contentTitleMap[STORE_RECOMMENDED] = tr("Recommended");
    contentIDMap[STORE_RECOMMENDED] = tr("Recommended");
    contentTitleMap[STORE_CATEGORIES] = tr("Categories");
    contentIDMap[STORE_CATEGORIES] = tr("Categories");
    contentTitleMap[APPS_INSTALLED] = tr("Installed");
    contentIDMap[APPS_INSTALLED] = tr("Installed");
    contentTitleMap[APPS_UPDATES] = tr("Updates");
    contentIDMap[APPS_UPDATES] = tr("User-Updates");
    contentTitleMap[IN_PROGRESS] = tr("In Progress");
    contentIDMap[IN_PROGRESS] = tr("In Progress");
    contentTitleMap[SYSTEM_UPDATES] = tr("Updates");
    contentIDMap[SYSTEM_UPDATES] = tr("System-Updates");
    //-----------------------------------------------------------------
    setSpacing(1);
    setIconSize( QSize(CONTENT_LIST_ICON_SIZE, CONTENT_LIST_ICON_SIZE));
    //-----------------------------------------------------------------
    createTitle(tr("STORE"));
    createSubItem(STORE_LATEST);
    createSubItem(STORE_RECOMMENDED);
    createSubItem(STORE_CATEGORIES);
    //-----------------------------------------------------------------
    createTitle(tr("YOURS APPS"));
    createSubItem(APPS_INSTALLED);
    createSubItem(APPS_UPDATES);
    createSubItem(IN_PROGRESS);
    //-----------------------------------------------------------------
    createTitle(tr("SYSTEM"));
    createSubItem(SYSTEM_UPDATES);

    setMaximumWidth(CONTENT_LIST_ITEM_W);
    setAutoFillBackground(false);
    setStyleSheet(CONTENT_LIST_STYLESHEET);

    m_pkgMgrTrk = PackageManagerTracker::Instance();
    connect(m_pkgMgrTrk, SIGNAL(userUpdatablePackageListReady(
                                        const QVector<Package *> &)), this, SLOT(getUserUpdatablePackages(
                                                                                         const QVector<Package *> &)));

    connect(m_pkgMgrTrk, SIGNAL(systemUpdatablePackageListReady(
                                        const QVector<Package *> &)), this, SLOT(getSystemUpdatablePackages(
                                                                                         const QVector<Package *> &)));

    connect(m_pkgMgrTrk, SIGNAL(packageRemoved(const QString &)),this, SLOT(inProgressListUpdated()));
    connect(m_pkgMgrTrk, SIGNAL(packageInstalled(const QString &)),this, SLOT(inProgressListUpdated()));
    connect(m_pkgMgrTrk, SIGNAL(packageUpdated(const QString &)),this, SLOT(inProgressListUpdated()));
    connect(m_pkgMgrTrk, SIGNAL(packageTaskCanceled(const QString &)),this, SLOT(inProgressListUpdated()));
    connect(m_pkgMgrTrk, SIGNAL(progressFailed(const QString &,const QString&)),this, SLOT(inProgressListUpdated()));
    connect(m_pkgMgrTrk, SIGNAL(inProgressRequest()),this, SLOT(inProgressListUpdated()));
}

void ContentList::inProgressListUpdated(){
    auto pkgs = m_pkgMgrTrk->inProgressList();
    if(pkgs.size()) {
        pInProgressWidgetItem->setHidden(false);
        inProgressUline->setHidden(false);
    } else {
        pInProgressWidgetItem->setHidden(true);
        inProgressUline->setHidden(true);
    }
}

void ContentList::getUserUpdatablePackages(const QVector<Package *> &packageList) {
    pUserUpdatableWidgetItem->refreshStatus(packageList.size());
}

void ContentList::getSystemUpdatablePackages(const QVector<Package *> &packageList) {
    pSystemUpdatableWidgetItem->refreshStatus(packageList.size());
}

void ContentList::createTitle(QString title) {
    auto item = new EmptyWidgetItem(this);
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    item->setSizeHint(QSize(CONTENT_LIST_EMPTY_ITEM_W,CONTENT_LIST_EMPTY_ITEM_H));
    addItem(item);

    auto label = new QLabel(title,this);
    auto pal = QGuiApplication::palette();
    auto fgcolor =  pal.color(QPalette::Active, QPalette::WindowText);
    QString sheet = QString::fromLatin1(CONTENT_LIST_ITEM_STYLE).arg(fgcolor.name());
    label->setStyleSheet(sheet);
    label->setContentsMargins(CONTENT_LIST_EMPTY_LEFT_MARGIN,CONTENT_LIST_EMPTY_TOP_MARGIN,CONTENT_LIST_EMPTY_RIGHT_MARGIN,CONTENT_LIST_EMPTY_BOTTOM_MARGIN);
    label->setFont(QFont("default",CONTENT_LIST_TITLE_FONT_SIZE, QFont::Bold));
    setItemWidget(item,label);
}

PxQListWidgetItem * ContentList::createSubItem(int contentId) {
    QString iconName = "px-categories";
    PxQListWidgetItem *item;
    if (contentId == IN_PROGRESS) {
        iconName = "px-in_progress";
        pInProgressWidgetItem = new InProgressWidgetItem(contentIDMap[contentId], contentTitleMap[contentId],
                                                         QFont("default", CONTENT_LIST_SUBTITLE_FONT_SIZE), iconName,
                                                         this);
        item = (PxQListWidgetItem *)pInProgressWidgetItem;
    } else if(contentId == APPS_INSTALLED){
        iconName = "px-installed";
        pInstalledWidgetItem = new InstalledWidgetItem(contentIDMap[contentId], contentTitleMap[contentId],
                                                       QFont("default", CONTENT_LIST_SUBTITLE_FONT_SIZE), iconName,
                                                       this);
        item = (PxQListWidgetItem *)pInstalledWidgetItem;
    } else if(contentId == APPS_UPDATES){
        iconName = "px-updates";
        pUserUpdatableWidgetItem = new UserUpdatableWidgetItem(contentIDMap[contentId], contentTitleMap[contentId],
                                                               QFont("default", CONTENT_LIST_SUBTITLE_FONT_SIZE),
                                                               iconName,
                                                               this);
        item = (PxQListWidgetItem *)pUserUpdatableWidgetItem;
    } else if(contentId == SYSTEM_UPDATES){
        iconName = "px-updates";
        pSystemUpdatableWidgetItem = new SystemUpdatableWidgetItem(contentIDMap[contentId], contentTitleMap[contentId],
                                                                   QFont("default", CONTENT_LIST_SUBTITLE_FONT_SIZE),
                                                                   iconName, this);
        item = (PxQListWidgetItem *)pSystemUpdatableWidgetItem;
    } else if(contentId == STORE_LATEST){
        iconName = "px-new";
        pLatestWidgetItem = new LatestWidgetItem(contentIDMap[contentId], contentTitleMap[contentId],
                                                 QFont("default", CONTENT_LIST_SUBTITLE_FONT_SIZE), iconName, this);
        item = (PxQListWidgetItem *)pLatestWidgetItem;
    } else if(contentId == STORE_RECOMMENDED){
        iconName = "px-recommended";
        pRecommendedWidgetItem = new RecommendedWidgetItem(contentIDMap[contentId], contentTitleMap[contentId],
                                                           QFont("default", CONTENT_LIST_SUBTITLE_FONT_SIZE), iconName,
                                                           this);
        item = (PxQListWidgetItem *)pRecommendedWidgetItem;
    } else { // contentId == STORE_CATEGORIES
        pCategoriesWidgetItem = new CategoriesWidgetItem(contentIDMap[contentId], contentTitleMap[contentId],
                                                         QFont("default", CONTENT_LIST_SUBTITLE_FONT_SIZE), iconName,
                                                         nullptr);
        item = (PxQListWidgetItem *)pCategoriesWidgetItem;
    }
    item->setSizeHint(item->getCustomWidget()->minimumSizeHint());
    addItem(item);
    setItemWidget(item, item->getCustomWidget());
    viewMap[contentId]=item->getView();

    auto _uline = new EmptyWidgetItem(this);
    if (contentId == IN_PROGRESS) {
        inProgressUline = _uline;
        inProgressUline->setHidden(true);
    }
    _uline->setFlags(Qt::NoItemFlags);
    _uline->setSizeHint(QSize(CONTENT_LIST_ULINE_W, CONTENT_LIST_ULINE_H));
    auto _pxLine = new PxLineSeperator;
    addItem(_uline);
    setItemWidget(_uline, _pxLine);
    return item;
}

PxQScrollArea *ContentList::getItem(int contentId) {
    return viewMap[contentId];
}

void ContentList::setSelectedItem(QString name) {
    auto i = contentIDMap.constBegin();
    while (i != contentIDMap.constEnd()) {
        if(name == i.value()){
            item(i.key())->setSelected(true);
            setFocus();
            return;
        }
        ++i;
    }
}
