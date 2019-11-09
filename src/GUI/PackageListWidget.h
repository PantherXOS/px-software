//
// Created by hamzeh on 11/9/19.
//

#ifndef PX_SOFTWARE_PACKAGELISTWIDGET_H
#define PX_SOFTWARE_PACKAGELISTWIDGET_H

#include <QScrollArea>

#include "PxQScrollArea.h"
#include "PackageWidget.h"
#include "DataAccessLayer.h"

class PackageListWidget : public PxQScrollArea{

public:
    PackageListWidget(QVector<PKG::Package *> packages, int id, QString title) : PxQScrollArea(id,title){
        boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        update(packages);
        QWidget *widget=new QWidget;
        widget->setLayout(boxLayout);
        setWidgetResizable(true);
        setWidget(widget);
    };

    void update(QVector<PKG::Package *> packages){
        for(auto pkg:packages){
            PackageWidget *packageWidget = new PackageWidget(pkg, false);
            boxLayout->addWidget(packageWidget);
        }
    }
private:
    QBoxLayout *boxLayout;
};


#endif //PX_SOFTWARE_PACKAGELISTWIDGET_H
