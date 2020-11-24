//
// Created by hamzeh on 12/31/19.
//

#ifndef PX_SOFTWARE_PXSIDEBAR_H
#define PX_SOFTWARE_PXSIDEBAR_H
#include <QListWidget>
#include <QVector>
#include <sys/statvfs.h>

#include "PXSideBarItem.h"
#include "PXParamSettings.h"

class PXSideBar : public QListWidget{
Q_OBJECT
public:
    PXSideBar(QWidget *parent = nullptr) : QListWidget(parent){
        setMaximumWidth(SIDEBAR_WIDTH);
        setAutoFillBackground(false);
        setStyleSheet(SIDEBAR_STYLESHEET);
    }

    void addPxItem(PXSideBarItem *item){
        _items.push_back(item);
        item->setSizeHint(item->getCustomWidget()->minimumSizeHint());
        addItem(item);
        setItemWidget(item, item->getCustomWidget());
    }

    void setSelectedItem(QString name) {
        for(auto const item : _items){
            if(item->title() == name){
                item->setSelected(true);
                setFocus();
                return;
            }
        }
    }

    QVector<PXSideBarItem> items(){
        QVector<PXSideBarItem> itemList;
        for(auto const item : _items)
            itemList.push_back(*item);
        return itemList;
    }

private:
    QVector<PXSideBarItem *> _items;
};


#endif //PX_SOFTWARE_PXSIDEBAR_H
