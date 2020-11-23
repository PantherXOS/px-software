//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_CATEGORYVIEW_H
#define PX_SOFTWARE_CATEGORYVIEW_H
#include "PXWidget.h"
#include "PXContentWidget.h"
#include "PackageManagerTracker.h"
#include "CategoryWidget.h"

class CategoryView : public PXContentWidget{
public:
    CategoryView(const QString &title,
                 PXContentWidget *parent = nullptr) : PXContentWidget(title, parent) {
        auto m_pkgMgrTrk = PackageManagerTracker::Instance();
        auto cats = m_pkgMgrTrk->categoryList();
        for (auto cat : cats) {
            auto catLayout = new CategoryWidget(cat);
            categoryWidgets.push_back(catLayout);
        }
    }

    bool event(QEvent * event) override{
        if(event->type() == QEvent::Resize) {
            auto layout = new QGridLayout;
            int i = 0;
            for (auto cat : categoryWidgets) {
                if(this->width() - CONTENT_LIST_ITEM_W > (3 * cat->size().width()))
                    layout->addWidget(cat, i/3, i%3);
                else if(this->width() - CONTENT_LIST_ITEM_W > (2 * cat->size().width()))
                    layout->addWidget(cat, i/2, i%2);
                else
                    layout->addWidget(cat, i, 1);
                i++;
            }
            layout->setVerticalSpacing(0);
            auto widget=new PXWidget;
            widget->setLayout(layout);
            layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            setWidget(widget);
        };
        return PXContentWidget::event(event);
    }

private:
    QVector<CategoryWidget *> categoryWidgets;
};
#endif //PX_SOFTWARE_CATEGORYVIEW_H
