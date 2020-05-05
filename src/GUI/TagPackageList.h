//
// Created by hamzeh on 11/30/19.
//

#ifndef PX_SOFTWARE_TAGPACKAGELIST_H
#define PX_SOFTWARE_TAGPACKAGELIST_H

#include <QMovie>
#include <QBoxLayout>
#include "QLabel"
#include "PackageListWidgetItem.h"
#include "PxQScrollArea.h"
#include "PackageManager.h"
#include "QProgressIndicator.h"

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

        auto loading = new QProgressIndicator(this);
        loading->setFixedSize(VIEW_LOADING_ICON_SIZE,VIEW_LOADING_ICON_SIZE);
        loading->startAnimation();

        boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        boxLayout->setAlignment(Qt::AlignCenter);
        boxLayout->addWidget(loading);
        auto *widget=new PxQWidget;
        widget->setLayout(boxLayout);
        setWidgetResizable(true);
        setWidget(widget);
        taskId = m_pkgMgr->requestTagPackages(tag);
    };

private slots:

    void tagPackagesReadyHandler(const QUuid &_taskId, const QVector<Package *> &packages) {
        if(_taskId == taskId){
            boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
            boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            auto *widget=new PxQWidget;
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
                emptyLabel->setText(tr("No record found for Tag") + QString("=\"") + tag +"\"");
                emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
                boxLayout->addWidget(emptyLabel);
            }
        }
    }

    void taskFailedHandler(const QUuid &_taskId, const QString &message) {
        if(_taskId == taskId){
            auto emptyLabel = new QLabel;
            emptyLabel->setText(message);
            emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
            boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
            boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            boxLayout->addWidget(emptyLabel);
            auto *widget=new PxQWidget;
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
