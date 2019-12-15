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
#define ITEM_WIDTH 200
#define ULINE_ITEM_WIDTH 2
#define ICON_ITEM_SIZE 16
ContentList::ContentList(QListWidget *parent) : QListWidget(parent) {
    m_pkgMgrTrk = PackageManagerTracker::Instance();
    setSpacing(1);
    setIconSize( QSize(ICON_ITEM_SIZE,ICON_ITEM_SIZE));
    //-----------------------------------------------------------------
    createSeperator();
    createItem("STORE");
    createSubItem(STORE_LATEST);
    createSubItem(STORE_RECOMMENDED);
    createSubItem(STORE_CATEGORIES);
    //-----------------------------------------------------------------
    createSeperator();
    createItem("YOURS APPS");
    createSubItem(APPS_INSTALLED);
    createSubItem(APPS_UPDATES);
    createSubItem(IN_PROGRESS);
    //-----------------------------------------------------------------
//    createSeperator();
//    createItem("SYSTEM");
//    addItem(createSubItem(SYSTEM_UPDATES));

    setMaximumWidth(ITEM_WIDTH);
//    setAutoFillBackground(false);
//    setStyleSheet("background-color: transparent;");
}

void ContentList::createItem(QString title) {
    PxQListWidgetItem *item= new PxQListWidgetItem(0,title, QFont("default", 12,QFont::Bold));
    item->setFlags(Qt::NoItemFlags);
    addItem(item);
}

void ContentList::createSubItem(int contentId) {
    QString iconName = ":images/general/src/GUI/resources/items";
    if(contentId==SYSTEM_UPDATES) {
        iconName = ":images/general/src/GUI/resources/update";
    } else if(contentId==APPS_UPDATES) {
        iconName = ":images/general/src/GUI/resources/update";
    }
    PxQListWidgetItem *item = new PxQListWidgetItem(contentId,contentTitleMap[contentId],QFont("default", 11), QIcon(iconName));
    addItem(item);

    auto _uline = new PxQListWidgetItem(0,"", QFont());
    _uline->setFlags(Qt::NoItemFlags);
    _uline->setSizeHint(QSize(ITEM_WIDTH,ULINE_ITEM_WIDTH));
    auto _pxLine = new PxLineSeperator;
    addItem(_uline);
    setItemWidget(_uline,_pxLine);
}

void ContentList::createSeperator() {
    QListWidgetItem *seperatorItem= new QListWidgetItem(this);
    seperatorItem->setSizeHint(QSize(64, 6));
    seperatorItem->setFlags(Qt::NoItemFlags);
    addItem(seperatorItem);
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

void ContentList::setSelectedItem(QString name) {
    for(const auto m : contentTitleMap){
        if(name==m.second){
            item(m.first)->setSelected(true);
            setFocus();
            return;
        }
    }
}
