//
// Created by hamzeh on 10/28/19.
//
#include "ContentList.h"

map<int,QString> contentTitleMap = {{STORE_LATEST, "Latest"},
                                   {STORE_RECOMMENDED, "Recommended"},
                                   {STORE_CATEGORIES, "Categories"},
                                   {APPS_INSTALLED, "Installed"},
                                   {APPS_UPDATES, "Updates"},
                                   {SYSTEM_UPDATES, "Updates"}};

ContentList::ContentList() {
    itemList = new QListWidget();
    itemList->setSpacing(4);
    itemList->setIconSize( QSize(16,16));
    //-----------------------------------------------------------------
    itemList->addItem(createSeperator());
    itemList->addItem(createItem("STORE"));
    itemList->addItem(createSubItem(STORE_LATEST));
    itemList->addItem(createSubItem(STORE_RECOMMENDED));
    itemList->addItem(createSubItem(STORE_CATEGORIES));
    //-----------------------------------------------------------------
    itemList->addItem(createSeperator());
    itemList->addItem(createItem("YOURS APPS"));
    itemList->addItem(createSubItem(APPS_INSTALLED));
    itemList->addItem(createSubItem(APPS_UPDATES));
    //-----------------------------------------------------------------
    itemList->addItem(createSeperator());
    itemList->addItem(createItem("SYSTEM"));
    itemList->addItem(createSubItem(SYSTEM_UPDATES));

//    itemList->setAutoFillBackground(false);
//    itemList->setStyleSheet("background-color: transparent;");
}

QListWidget *ContentList::getItemList() {
    return itemList;
}

PxQListWidgetItem *ContentList::createItem(QString title) {
    PxQListWidgetItem *item= new PxQListWidgetItem(0,title, QFont("default", 12,QFont::Bold));
    item->setFlags(Qt::NoItemFlags);
    return item;
}

PxQListWidgetItem *ContentList::createSubItem(int contentId) {
    QString iconName = ":images/general/src/GUI/resources/items";
    QGridLayout *layout = new QGridLayout;

    QString m_dbPath = "./SAMPLE_DB";
    PKG::DataAccessLayer dbLayer(m_dbPath);
    auto cats = dbLayer.categoryList();
    switch(contentId){
        case STORE_LATEST:{
            QLabel *label = new QLabel();
            label->setText("TBD - STORE_LATEST");
            layout->addWidget(label);
        }
            break;
        case STORE_RECOMMENDED:{
            QLabel *label = new QLabel();
            label->setText("TBD - STORE_RECOMMENDED");
            layout->addWidget(label);
        }
            break;
        case STORE_CATEGORIES: {
            int i=0;
            for (auto cat : cats) {
                CategoryLayout *catLayout = new CategoryLayout(cat);
                layout->addWidget(catLayout, i++, 0);
            }
        }
            break;
        case APPS_INSTALLED:{
            QLabel *label = new QLabel();
            label->setText("TBD - APPS_INSTALLED");
            layout->addWidget(label);
        }
            break;
        case APPS_UPDATES:{
            QLabel *label = new QLabel();
            label->setText("TBD - APPS_UPDATES");
            layout->addWidget(label);
        }
            iconName = ":images/general/src/GUI/resources/update";
            break;
        case SYSTEM_UPDATES:{
            QLabel *label = new QLabel();
            label->setText("TBD - SYSTEM_UPDATES");
            layout->addWidget(label);
        }
            iconName = ":images/general/src/GUI/resources/update";
            break;
        default:
            break;
    }

    QWidget *widget=new QWidget;
    widget->setLayout(layout);

    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    PxQScrollArea * scrollArea = new PxQScrollArea(contentId,contentTitleMap[contentId]);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);

    PxQListWidgetItem *item = new PxQListWidgetItem(contentId,contentTitleMap[contentId],QFont("default", 11), QIcon(iconName));
    widgetsMap[contentId]=scrollArea;
    return item;
}

QListWidgetItem *ContentList::createSeperator() {
    QListWidgetItem *seperatorItem= new QListWidgetItem();
    seperatorItem->setSizeHint(QSize(64, 6));
    seperatorItem->setFlags(Qt::NoItemFlags);
    return seperatorItem;
}

PxQScrollArea *ContentList::getItem(int id) {
    return widgetsMap[id];
}
