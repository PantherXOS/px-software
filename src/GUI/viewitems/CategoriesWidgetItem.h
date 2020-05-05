//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_CATEGORIESWIDGETITEM_H
#define PX_SOFTWARE_CATEGORIESWIDGETITEM_H
#include "PxQScrollArea.h"
#include "PxQListWidgetItem.h"
#include "CategoryWidget.h"

class CategoriesWidgetItem : public PxQListWidgetItem{
public:
    CategoriesWidgetItem(const QString &id, const QString &title, const QFont &font, const QString &iconItemFile,
                         QListWidget *parent = nullptr) : PxQListWidgetItem(id, title, font, iconItemFile,
                                                                            parent) {
        this->id=id;
        this->title=title;
    }

    PxQScrollArea *getView() override{
        auto layout = new QGridLayout;
        auto m_pkgMgrTrk = PackageManagerTracker::Instance();
        auto cats = m_pkgMgrTrk->categoryList();
        int i = 0;
        for (auto cat : cats) {
            auto catLayout = new CategoryWidget(cat);
            layout->addWidget(catLayout, i/3, i%3);
            i++;
        }

        auto widget=new PxQWidget;
        widget->setLayout(layout);
        layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        view = new PxQScrollArea(title);
        view->setWidget(widget);

        return view;
    }

private:
    QString id;
    QString title;
    PxQScrollArea *view;
};
#endif //PX_SOFTWARE_CATEGORIESWIDGETITEM_H
