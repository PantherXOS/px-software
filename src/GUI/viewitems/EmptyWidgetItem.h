//
// Created by hamzeh on 12/18/19.
//

#ifndef PX_SOFTWARE_EMPTYWIDGETITEM_H
#define PX_SOFTWARE_EMPTYWIDGETITEM_H
#include <QListWidgetItem>
#include <QScrollArea>

// TODO When I can to click on title item in the left panel and find out that
//  this item is non-selectable, This class should be removed.
class EmptyWidgetItem : public QListWidgetItem{
public:
    EmptyWidgetItem(QListWidget *parent = nullptr) : QListWidgetItem (parent) {
    }

    QScrollArea *getView() {
        return nullptr;
    }
};
#endif //PX_SOFTWARE_EMPTYWIDGETITEM_H
