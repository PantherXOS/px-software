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
    setSpacing(1);
    setIconSize( QSize(CONTENT_LIST_ICON_SIZE, CONTENT_LIST_ICON_SIZE));
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
    updateWidgetItem = createSubItem(APPS_UPDATES);
//    updateWidgetItem->refreshNumber(10);
    createSubItem(IN_PROGRESS);
    //-----------------------------------------------------------------
//    createSeperator();
//    createItem("SYSTEM");
//    createSystemUpdateItem(SYSTEM_UPDATES);

    setMaximumWidth(CONTENT_LIST_ITEM_W);
//    setAutoFillBackground(false);
//    setStyleSheet(CONTENT_LIST_STYLESHEET);
    connect(m_pkgMgrTrk, SIGNAL(userUpdatablePackageListReady(
                                        const QVector<Package *> &)), this, SLOT(getUserUpdatablePackages(
                                                                                         const QVector<Package *> &)));
    createContents();
}

void ContentList::getUserUpdatablePackages(const QVector<Package *> &packageList) {
    updateWidgetItem->refreshNumber(packageList.size());
}

void ContentList::createItem(QString title) {
    auto item = new PxQListWidgetItem(0, title, QFont(), "");
    item->setFlags(Qt::NoItemFlags);
    addItem(item);

    auto label = new QLabel(title,this);
    label->setFont(QFont("default",CONTENT_LIST_TITLE_FONT_SIZE, QFont::Bold));
    setItemWidget(item,label);
}

PxQListWidgetItem * ContentList::createSubItem(int contentId) {
    QString iconName;
    if(contentId == APPS_UPDATES || contentId == SYSTEM_UPDATES)
        iconName = ":images/general/src/GUI/resources/update";
    else
        iconName = ":images/general/src/GUI/resources/items";
    auto item = new PxQListWidgetItem(contentId, contentTitleMap[contentId], QFont("default", CONTENT_LIST_SUBTITLE_FONT_SIZE), iconName);
    item->setSizeHint(item->getCustomWidget()->minimumSizeHint());
    addItem(item);
    setItemWidget(item,item->getCustomWidget());

    auto _uline = new QListWidgetItem(this);
    _uline->setFlags(Qt::NoItemFlags);
    _uline->setSizeHint(QSize(CONTENT_LIST_ULINE_W, CONTENT_LIST_ULINE_H));
    auto _pxLine = new PxLineSeperator;
    addItem(_uline);
    setItemWidget(_uline,_pxLine);
    return item;
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
        installedPackageListView->refresh();
        return installedPackageListView;
    } else if (contentId == APPS_UPDATES) {
        userUpdatablePackageListView->refresh();
        return userUpdatablePackageListView;
    } else if (contentId == SYSTEM_UPDATES) {
        systemUpdatablePackageListView->refresh();
        return systemUpdatablePackageListView;
    } else if(contentId == IN_PROGRESS) {
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

void ContentList::createContents() {
    InstalledPackageListView::init(contentTitleMap[APPS_INSTALLED]);
    installedPackageListView = InstalledPackageListView::Instance();
    installedPackageListView->refresh();

    UserUpdatablePackageListView::init(contentTitleMap[APPS_UPDATES]);
    userUpdatablePackageListView= UserUpdatablePackageListView::Instance();
    userUpdatablePackageListView->refresh();

    SystemUpdatablePackageListView::init(contentTitleMap[SYSTEM_UPDATES]);
    systemUpdatablePackageListView = SystemUpdatablePackageListView::Instance();
    systemUpdatablePackageListView->refresh();

    InProgressPackageListView::init(contentTitleMap[IN_PROGRESS]);
    inProgressPakcageListView = InProgressPackageListView::Instance();
    inProgressPakcageListView->refresh();
}
