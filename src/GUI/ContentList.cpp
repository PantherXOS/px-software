//
// Created by hamzeh on 10/28/19.
//
#include "ContentList.h"

map<int,QString> contentTitleMap = {{STORE_LATEST, "Latest"},
                                   {STORE_RECOMMENDED, "Recommended"},
                                   {STORE_CATEGORIES, "Categories"},
                                   {APPS_INSTALLED, "Installed"},
                                   {APPS_UPDATES, "Updates"},
                                   {IN_PROGRESS, "In Progress"},
                                   {SYSTEM_UPDATES, "Updates"}};

ContentList::ContentList(QListWidget *parent) : QListWidget(parent) {
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    setSpacing(4);
    setIconSize( QSize(16,16));
    //-----------------------------------------------------------------
    addItem(createSeperator());
    addItem(createItem("STORE"));
    addItem(createSubItem(STORE_LATEST));
    addItem(createSubItem(STORE_RECOMMENDED));
    addItem(createSubItem(STORE_CATEGORIES));
    //-----------------------------------------------------------------
    addItem(createSeperator());
    addItem(createItem("YOURS APPS"));
    addItem(createSubItem(APPS_INSTALLED));
    addItem(createSubItem(APPS_UPDATES));
    addItem(createSubItem(IN_PROGRESS));
    //-----------------------------------------------------------------
    addItem(createSeperator());
    addItem(createItem("SYSTEM"));
    addItem(createSubItem(SYSTEM_UPDATES));

    setMaximumWidth(200);
//    setAutoFillBackground(false);
//    setStyleSheet("background-color: transparent;");
//    InstalledPackageListView::init(contentTitleMap[APPS_INSTALLED]);
//    UserUpdatablePackageListView::init(contentTitleMap[APPS_UPDATES]);
//    InProgressPackageListView::init(contentTitleMap[IN_PROGRESS]);
//    SystemUpdatablePackageListView::init(contentTitleMap[SYSTEM_UPDATES]);
}

PxQListWidgetItem *ContentList::createItem(QString title) {
    PxQListWidgetItem *item= new PxQListWidgetItem(0,title, QFont("default", 12,QFont::Bold));
    item->setFlags(Qt::NoItemFlags);
    return item;
}

PxQListWidgetItem *ContentList::createSubItem(int contentId) {
    QString iconName = ":images/general/src/GUI/resources/items";
    if(contentId==SYSTEM_UPDATES) {
        iconName = ":images/general/src/GUI/resources/update";
    } else if(contentId==APPS_UPDATES) {
        iconName = ":images/general/src/GUI/resources/update";
    }
    PxQListWidgetItem *item = new PxQListWidgetItem(contentId,contentTitleMap[contentId],QFont("default", 11), QIcon(iconName));
    return item;
}

QListWidgetItem *ContentList::createSeperator() {
    QListWidgetItem *seperatorItem= new QListWidgetItem();
    seperatorItem->setSizeHint(QSize(64, 6));
    seperatorItem->setFlags(Qt::NoItemFlags);
    return seperatorItem;
}

PxQScrollArea *ContentList::getItem(int contentId) {
    PxQScrollArea * scrollArea;
    if(contentId == APPS_INSTALLED) {
        InstalledPackageListView::init(contentTitleMap[APPS_INSTALLED]);
        InstalledPackageListView * installedPackageListView = InstalledPackageListView::Instance();
        installedPackageListView->refresh();
        return installedPackageListView;
    } else if (contentId == APPS_UPDATES) {
        UserUpdatablePackageListView::init(contentTitleMap[APPS_UPDATES]);
        UserUpdatablePackageListView * userUpdatablePackageListView = UserUpdatablePackageListView::Instance();
        userUpdatablePackageListView->refresh();
        return userUpdatablePackageListView;
    } else if (contentId == SYSTEM_UPDATES) {
        SystemUpdatablePackageListView::init(contentTitleMap[SYSTEM_UPDATES]);
        SystemUpdatablePackageListView * systemUpdatablePackageListView = SystemUpdatablePackageListView::Instance();
        systemUpdatablePackageListView->refresh();
        return systemUpdatablePackageListView;
    } else if(contentId == IN_PROGRESS) {
        InProgressPackageListView::init(contentTitleMap[IN_PROGRESS]);
        InProgressPackageListView *inProgressPakcageListView = InProgressPackageListView::Instance();
        inProgressPakcageListView->refresh();
        return inProgressPakcageListView;
    } else if(contentId == STORE_RECOMMENDED){
        auto view = new TagPackageList(contentTitleMap[contentId], "recommended");
        return view;
    } else if(contentId == STORE_LATEST) {
        auto view = new TagPackageList(contentTitleMap[contentId], "latest");
        return view;
    } else {
        QGridLayout *layout = new QGridLayout;
        if(contentId == STORE_CATEGORIES) {
            auto cats = m_pkgMgrTrk->categoryList();
            int i = 0;
            for (auto cat : cats) {
                CategoryWidget *catLayout = new CategoryWidget(cat);
                layout->addWidget(catLayout, i++, 0);
            }
        }
        QWidget *widget=new QWidget;
        widget->setLayout(layout);
//        widget->showMaximized();

        layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        scrollArea = new PxQScrollArea(contentTitleMap[contentId], nullptr);
        scrollArea->setWidget(widget);
    }
    return scrollArea;
}
