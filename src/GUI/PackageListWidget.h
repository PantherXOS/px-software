//
// Created by hamzeh on 11/9/19.
//

#ifndef PX_SOFTWARE_PACKAGELISTWIDGET_H
#define PX_SOFTWARE_PACKAGELISTWIDGET_H

#include <QScrollArea>
#include <QMovie>

#include "PxQScrollArea.h"
#include "PackageListWidgetItem.h"

class PackageListWidget : public PxQScrollArea{

public:
    PackageListWidget(QVector<Package *> &packages, bool removeEnable, int id, QString title,
                      PxQScrollArea *parent = nullptr) : PxQScrollArea(id, title, parent){
        this->removeEnable=removeEnable;
        QMovie *movie = new QMovie(":images/general/src/GUI/resources/loading.gif");
        QSize size(128,128);
        movie->setScaledSize(size);
        setAlignment(Qt::AlignCenter);
        QLabel *processLabel = new QLabel(this);
        processLabel->setMovie(movie);
        processLabel->setFixedSize(size);
        movie->start();
        setWidget(processLabel);
//        update(packages);
    };

    void update(QVector<PKG::Package *> packages) {
        if(boxLayout!=nullptr)
            delete boxLayout;
        boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        QWidget *widget=new QWidget;
        widget->setLayout(boxLayout);
        setWidgetResizable(true);
        setWidget(widget);
        for(auto pkg:packages) {
            PackageListWidgetItem *packageWidget = new PackageListWidgetItem(pkg, removeEnable);
            boxLayout->addWidget(packageWidget);
        }
    }

    void remove(QWidget *widget){
        boxLayout->removeWidget(widget);
    }
private:
    QBoxLayout *boxLayout=nullptr;
    bool removeEnable;
};


#endif //PX_SOFTWARE_PACKAGELISTWIDGET_H
