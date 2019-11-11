//
// Created by hamzeh on 11/9/19.
//

#ifndef PX_SOFTWARE_PACKAGELISTWIDGET_H
#define PX_SOFTWARE_PACKAGELISTWIDGET_H

#include <QScrollArea>

#include "PxQScrollArea.h"
#include "PackageWidget.h"

class PackageListWidget : public PxQScrollArea{

public:
    PackageListWidget(QVector<PKG::Package *> packages, bool removeEnable, int id, QString title, PxQScrollArea * parent= nullptr) : PxQScrollArea(id,title, parent){
        this->removeEnable=removeEnable;
        update(packages);
    };

    void update(QVector<PKG::Package *> packages){
        if(boxLayout!=nullptr)
            delete boxLayout;
        boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        QWidget *widget=new QWidget;
        widget->setLayout(boxLayout);
        setWidgetResizable(true);
        setWidget(widget);

        for(auto pkg:packages){
            PackageWidget *packageWidget = new PackageWidget(pkg, removeEnable);
            boxLayout->addWidget(packageWidget);
        }
    }
private:
    QBoxLayout *boxLayout=nullptr;
    bool removeEnable;
};


#endif //PX_SOFTWARE_PACKAGELISTWIDGET_H
