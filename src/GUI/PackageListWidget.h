//
// Created by hamzeh on 11/9/19.
//

#ifndef PX_SOFTWARE_PACKAGELISTWIDGET_H
#define PX_SOFTWARE_PACKAGELISTWIDGET_H

#include <QScrollArea>
#include <QMovie>

#include "PxQScrollArea.h"
#include "PackageListWidgetItem.h"
#include "PackageManager.h"

class PackageListWidget : public PxQScrollArea{
    Q_OBJECT
public:
    PackageListWidget(bool removeEnable, int id, const QString &title,
                      PxQScrollArea *parent = nullptr) : PxQScrollArea(id, title, parent){
        PackageManager *m_pkgMgr = PackageManager::Instance();
        connect(m_pkgMgr, SIGNAL(taskFailed(const QUuid &, const QString &)),this, SLOT(taskFailedHandler(const QUuid &, const QString &)));
        connect(m_pkgMgr, SIGNAL(categoryPackagesReady(const QUuid &,const QVector<Package *> &)),this, SLOT(categoryPackagesReadyHandler(const QUuid &,const QVector<Package *> &)));
        m_pkgMgr->requestCategoryPackages(title);

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
    };

private slots:
    void categoryPackagesReadyHandler(const QUuid &taskId,const QVector<Package *> & packages){
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

    void taskFailedHandler(const QUuid &taskId, const QString & message) {
        qDebug() << this << " : "<< message;
    }

private:
    QBoxLayout *boxLayout=nullptr;
    bool removeEnable;
};


#endif //PX_SOFTWARE_PACKAGELISTWIDGET_H
