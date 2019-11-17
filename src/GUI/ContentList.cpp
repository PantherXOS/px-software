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
    m_pkgMgr = PackageManager::Instance();
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
        m_pkgMgr->requestInstalledPackages();
        connect(m_pkgMgr, SIGNAL(installedPackagesReady(
                                         const QUuid &,
                                         const QVector<Package *>)), this, SLOT(getInstalledPackages(
                                                                                        const QUuid &, const QVector<Package *>)));
        QVector<Package *> pkgs;
        installedPackageList = new PackageListWidget(pkgs, true, APPS_INSTALLED, contentTitleMap[APPS_INSTALLED]);
        return installedPackageList;
    } else if(contentId == IN_PROGRESS){
        DataAccessLayer *dbLayer = new DataAccessLayer("./SAMPLE_DB");
        QVector<Package *> pkgs;
        PackageManagerTracker *m_pkgMngrTrk = PackageManagerTracker::Instance();
        QStringList list = m_pkgMngrTrk->getList();
        for(const auto &l: list) {
            auto *pkg = dbLayer->packageDetails(l);
            pkgs.append(pkg);
        }
        inProgressPackageList = new PackageListWidget(pkgs, true, IN_PROGRESS, contentTitleMap[IN_PROGRESS]);
        return inProgressPackageList;
    } else {
        QGridLayout *layout = new QGridLayout;
        QLabel *label = new QLabel();
        if(contentId == STORE_CATEGORIES) {
            auto cats = m_pkgMgr->categoryList();
            int i = 0;
            for (auto cat : cats) {
                CategoryWidget *catLayout = new CategoryWidget(cat);
                layout->addWidget(catLayout, i++, 0);
            }
        } else {
            label->setText(contentTitleMap[contentId] + " : Not Implemented Yet !");
            layout->addWidget(label);
        }
        QWidget *widget=new QWidget;
        widget->setLayout(layout);
//        widget->showMaximized();

        layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        scrollArea = new PxQScrollArea(contentId,contentTitleMap[contentId]);
        scrollArea->setWidget(widget);
    }
    return scrollArea;
}

void ContentList::getInstalledPackages(const QUuid &taskId, const QVector<Package *> &packageList){
    installedPackageList->update(packageList);
}
