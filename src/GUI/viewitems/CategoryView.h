//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_CATEGORYVIEW_H
#define PX_SOFTWARE_CATEGORYVIEW_H
#include "PXContentWidget.h"

#include <QListWidget>

#include "PackageManagerTracker.h"
#include "CategoryWidget.h"

class CategoryView : public PXContentWidget{
    Q_OBJECT
public:
    CategoryView(const QString &title,
                 PXContentWidget *parent = nullptr) : PXContentWidget(title, parent) {
        auto m_pkgMgrTrk = PackageManagerTracker::Instance();
        _listWidget = new QListWidget;
        connect(_listWidget, &QListWidget::itemPressed, [&](QListWidgetItem *item){
            auto catItem = dynamic_cast<CategoryWidgetItem*>(item);
            if(catItem){
                emit categoryItemClicked(catItem->widget()->getCategory()->name());
                qInfo() << "Category Clicked:" << catItem->widget()->getCategory()->name();
            }
        });    
        _listWidget->setFlow(QListView::LeftToRight);
        _listWidget->setViewMode(QListView::IconMode);
        _listWidget->setResizeMode(QListView::Adjust);
        _listWidget->setFrameStyle(QFrame::NoFrame);
        _listWidget->viewport()->setAcceptDrops(false);
        _listWidget->setDropIndicatorShown(false);
        _listWidget->setContentsMargins(0,0,0,0);
        auto cats = m_pkgMgrTrk->categoryList();        
        for (auto cat : cats) {
            auto item = new CategoryWidgetItem(cat);
            _listWidget->addItem(item);
            _listWidget->setItemWidget(item, item->widget());
            item->setSizeHint(item->widget()->size());
            _categoryWidgetItems.push_back(item);
        }
        
        auto layout = new QVBoxLayout;
        layout->addWidget(_listWidget);
        layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        layout->setContentsMargins(0,5,0,5);
        setLayout(layout);
    }

    bool event(QEvent * event) override{
        if(!_firstDecoration && event->type() == QEvent::Resize) {
            _firstDecoration = true;
            if( width()/3 >= CATEGORY_ITEM_WIDTH) {
                _listWidget->setGridSize(QSize(width()/3 - 10, CATEGORY_ICON_SIZE + 12));
                for(auto const &i: _categoryWidgetItems){
                    i->widget()->setFixedWidth(width()/3 - 40);
                    i->setSizeHint(i->widget()->size());
                }
            }
        };
        return PXContentWidget::event(event);
    }

signals:
    void categoryItemClicked(const QString &category);

private:
    bool         _firstDecoration = false;
    QListWidget *_listWidget = nullptr;
    QVector<CategoryWidgetItem *> _categoryWidgetItems;    
};
#endif //PX_SOFTWARE_CATEGORYVIEW_H
