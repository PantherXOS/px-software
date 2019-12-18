//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_CATEGORIESWIDGETITEM_H
#define PX_SOFTWARE_CATEGORIESWIDGETITEM_H
#include "src/GUI/px-qobjects/PxQScrollArea.h"
#include "src/GUI/px-qobjects/PxQListWidgetItem.h"
#include "CategoryWidget.h"

class CategoriesWidgetItem : public PxQListWidgetItem{
public:
    CategoriesWidgetItem(const QString &title, const QFont &font, const QString &iconItemFile,
                         QListWidget *parent = nullptr) : PxQListWidgetItem(title, font, iconItemFile, parent) {
        QGridLayout *layout = new QGridLayout;
        auto m_pkgMgrTrk = PackageManagerTracker::Instance();
        auto cats = m_pkgMgrTrk->categoryList();
        int i = 0;
        for (auto cat : cats) {
            CategoryWidget *catLayout = new CategoryWidget(cat);
            layout->addWidget(catLayout, i++, 0);
        }

        QWidget *widget=new QWidget;
        widget->setLayout(layout);
        layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        view = new PxQScrollArea(title);
        view->setWidget(widget);
    }

    PxQScrollArea *getView() override{
        return view;
    }

private:
    PxQScrollArea *view;
};
#endif //PX_SOFTWARE_CATEGORIESWIDGETITEM_H
