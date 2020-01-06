//
// Created by hamzeh on 11/9/19.
//

#ifndef PX_SOFTWARE_PACKAGELISTWIDGET_H
#define PX_SOFTWARE_PACKAGELISTWIDGET_H

#include <QScrollArea>
#include <QMovie>

#include "PxQWidget.h"
#include "PxQScrollArea.h"
#include "PackageListWidgetItem.h"
#include "PackageManager.h"
#include "PxViewLoadingAnimation.h"

class PackageListWidget : public PxQScrollArea{
    Q_OBJECT
public:
    PackageListWidget(bool removeEnable, const QString &title, PxQScrollArea *parent = nullptr) : PxQScrollArea(title, parent) {
        PackageManager *m_pkgMgr = PackageManager::Instance();
        connect(m_pkgMgr, SIGNAL(taskFailed(const QUuid &, const QString &)),this, SLOT(taskFailedHandler(const QUuid &, const QString &)));
        connect(m_pkgMgr, SIGNAL(categoryPackagesReady(const QUuid &,const QVector<Package *> &)),this, SLOT(categoryPackagesReadyHandler(const QUuid &,const QVector<Package *> &)));
        this->removeEnable=removeEnable;
        auto loading = new PxViewLoadingAnimation(this);
        setAlignment(Qt::AlignCenter);
        setWidget(loading);
        m_pkgMgr->requestCategoryPackages(title);
    };

private slots:
    void categoryPackagesReadyHandler(const QUuid &taskId,const QVector<Package *> & packages){
        if(boxLayout!=nullptr)
            delete boxLayout;

        boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        auto *widget=new PxQWidget;
        widget->setLayout(boxLayout);
        setWidgetResizable(true);
        setWidget(widget);
        for(auto pkg:packages) {
            PackageListWidgetItem *packageWidget = new PackageListWidgetItem(pkg, removeEnable , this);
            packageWidget->setContentsMargins(0,0,0,10);
            boxLayout->addWidget(packageWidget);
        }
    }

    void taskFailedHandler(const QUuid &taskId, const QString & message) {
        qDebug() << this << " : "<< message;
    }

private:
    QBoxLayout *boxLayout=nullptr;
    bool removeEnable;
};


#endif //PX_SOFTWARE_PACKAGELISTWIDGET_H
