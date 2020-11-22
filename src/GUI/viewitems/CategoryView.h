//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_CATEGORYVIEW_H
#define PX_SOFTWARE_CATEGORYVIEW_H
#include "PXWidget.h"
#include "PxQScrollArea.h"
#include "PackageManagerTracker.h"
#include "CategoryWidget.h"

class CategoryView : public PxQScrollArea{
public:
    CategoryView(const QString &title,
                 PxQScrollArea *parent = nullptr) : PxQScrollArea(title, parent) {
        auto layout = new QGridLayout;
        auto m_pkgMgrTrk = PackageManagerTracker::Instance();
        auto cats = m_pkgMgrTrk->categoryList();
        int i = 0;
        for (auto cat : cats) {
            auto catLayout = new CategoryWidget(cat);
            layout->addWidget(catLayout, i/3, i%3);
            i++;
        }

        auto widget=new PXWidget;
        widget->setLayout(layout);
        layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        setWidget(widget);
    }
};
#endif //PX_SOFTWARE_CATEGORYVIEW_H
