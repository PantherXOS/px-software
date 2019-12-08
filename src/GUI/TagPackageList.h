//
// Created by hamzeh on 11/30/19.
//

#ifndef PX_SOFTWARE_TAGPACKAGELIST_H
#define PX_SOFTWARE_TAGPACKAGELIST_H

#include <QMovie>
#include <QBoxLayout>
#include <PackageListWidgetItem.h>

#include "PxQScrollArea.h"
#include "PackageManager.h"
#include "QLabel"

using namespace PKG;
class TagPackageList : public PxQScrollArea {
Q_OBJECT
public:
    TagPackageList(const QString &title, const QString &tag, PxQScrollArea *parent = nullptr) : PxQScrollArea(title, parent) {
        this->tag=tag;
        PackageManager *m_pkgMgr = PackageManager::Instance();
        connect(m_pkgMgr, SIGNAL(taskFailed(
                                         const QUuid &, const QString &)), this, SLOT(taskFailedHandler(
                                                                                              const QUuid &, const QString &)));
        connect(m_pkgMgr, SIGNAL(tagPackagesReady(
                                         const QUuid &, const QVector<Package *> &)), this,
                SLOT(tagPackagesReadyHandler(
                             const QUuid &, const QVector<Package *> &)));

        QMovie *movie = new QMovie(":images/general/src/GUI/resources/loading.gif");
        QSize size(128, 128);
        movie->setScaledSize(size);
        setAlignment(Qt::AlignCenter);
        QLabel *processLabel = new QLabel(this);
        processLabel->setMovie(movie);
        processLabel->setFixedSize(size);
        movie->start();
        setWidget(processLabel);
        taskId = m_pkgMgr->requestTagPackages(tag);
    };

private slots:

    void tagPackagesReadyHandler(const QUuid &taskId, const QVector<Package *> &packages) {
        boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        QWidget *widget = new QWidget;
        widget->setLayout(boxLayout);
        setWidgetResizable(true);
        setWidget(widget);
        if(packages.size()){
            for (auto pkg:packages) {
                auto packageWidget = new PackageListWidgetItem(pkg, false, this);
                boxLayout->addWidget(packageWidget);
            }
        } else {
            auto emptyLabel = new QLabel;
            emptyLabel->setText("No record found for Tag=\"" + tag +"\"");
            emptyLabel->setFont(QFont("default", 16));
            boxLayout->addWidget(emptyLabel);
        }
    }

    void taskFailedHandler(const QUuid &_taskId, const QString &message) {
        if(_taskId == taskId){
            auto emptyLabel = new QLabel;
            emptyLabel->setText(message);
            emptyLabel->setFont(QFont("default", 16));
            boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
            boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            boxLayout->addWidget(emptyLabel);
            QWidget *widget=new QWidget;
            widget->setLayout(boxLayout);
            setWidgetResizable(true);
            setWidget(widget);
        }
    }

private:
    QBoxLayout *boxLayout = nullptr;
    QString tag;
    QUuid taskId;
};

#endif //PX_SOFTWARE_TAGPACKAGELIST_H
