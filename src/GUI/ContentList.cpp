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
//    createTitle("SYSTEM");
//    createSystemUpdateItem(SYSTEM_UPDATES);

    setMaximumWidth(CONTENT_LIST_ITEM_W);
//    setAutoFillBackground(false);
//    setStyleSheet(CONTENT_LIST_STYLESHEET);
    auto m_pkgMgrTrk = PackageManagerTracker::Instance();
    connect(m_pkgMgrTrk, SIGNAL(userUpdatablePackageListReady(
                                        const QVector<Package *> &)), this, SLOT(getUserUpdatablePackages(
                                                                                         const QVector<Package *> &)));
}

void ContentList::getUserUpdatablePackages(const QVector<Package *> &packageList) {
    pUserUpdatableWidgetItem->refreshStatus(packageList.size());
}

void ContentList::createTitle(QString title) {
    auto item = new EmptyWidgetItem(this);
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    item->setSizeHint(QSize(CONTENT_LIST_EMPTY_ITEM_W,CONTENT_LIST_EMPTY_ITEM_H));
    addItem(item);

    auto label = new QLabel(title,this);
    label->setContentsMargins(CONTENT_LIST_EMPTY_LEFT_MARGIN,CONTENT_LIST_EMPTY_TOP_MARGIN,CONTENT_LIST_EMPTY_RIGHT_MARGIN,CONTENT_LIST_EMPTY_BOTTOM_MARGIN);
    label->setFont(QFont("default",CONTENT_LIST_TITLE_FONT_SIZE, QFont::Bold));
    setItemWidget(item,label);
}

PxQListWidgetItem * ContentList::createSubItem(int contentId) {
    QString iconName = ":images/general/src/GUI/resources/items";
    PxQListWidgetItem *item;
    if (contentId == IN_PROGRESS) {
        pInProgressWidgetItem = new InProgressWidgetItem(tr("In Progress"),
                                                         QFont("default", CONTENT_LIST_SUBTITLE_FONT_SIZE), iconName,
                                                         this);
        item = (PxQListWidgetItem *)pInProgressWidgetItem;
    } else if(contentId == APPS_INSTALLED){
        pInstalledWidgetItem = new InstalledWidgetItem(tr("Installed"),
                                                       QFont("default", CONTENT_LIST_SUBTITLE_FONT_SIZE), iconName,
                                                       this);
        item = (PxQListWidgetItem *)pInstalledWidgetItem;
    } else if(contentId == APPS_UPDATES){
        iconName = ":images/general/src/GUI/resources/update";
        pUserUpdatableWidgetItem = new UserUpdatableWidgetItem(tr("Updates"),
                                                               QFont("default", CONTENT_LIST_SUBTITLE_FONT_SIZE), iconName,
                                                               this);
        item = (PxQListWidgetItem *)pUserUpdatableWidgetItem;
    } else if(contentId == SYSTEM_UPDATES){
        iconName = ":images/general/src/GUI/resources/update";
        pSystemUpdatableWidgetItem = new SystemUpdatableWidgetItem(tr("Updates"),
                                                                   QFont("default", CONTENT_LIST_SUBTITLE_FONT_SIZE),
                                                                   iconName, this);
        item = (PxQListWidgetItem *)pSystemUpdatableWidgetItem;
    } else if(contentId == STORE_LATEST){
        pLatestWidgetItem = new LatestWidgetItem(tr("Latest"),
                                                 QFont("default", CONTENT_LIST_SUBTITLE_FONT_SIZE), iconName, this);
        item = (PxQListWidgetItem *)pLatestWidgetItem;
    } else if(contentId == STORE_RECOMMENDED){
        pRecommendedWidgetItem = new RecommendedWidgetItem(tr("Recommended"),
                                                           QFont("default", CONTENT_LIST_SUBTITLE_FONT_SIZE), iconName,
                                                           this);
        item = (PxQListWidgetItem *)pRecommendedWidgetItem;
    } else { // contentId == STORE_CATEGORIES
        pCategoriesWidgetItem = new CategoriesWidgetItem(tr("Categories"),
                                                         QFont("default", CONTENT_LIST_SUBTITLE_FONT_SIZE), iconName,
                                                         nullptr);
        item = (PxQListWidgetItem *)pCategoriesWidgetItem;
    }
    item->setSizeHint(item->getCustomWidget()->minimumSizeHint());
    addItem(item);
    setItemWidget(item, item->getCustomWidget());
    viewMap[contentId]=item->getView();

    auto _uline = new EmptyWidgetItem(this);
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
    for(const auto &m : contentTitleMap){
        if(name==m.second){
            item(m.first)->setSelected(true);
            setFocus();
            return;
        }
    }
}
