//
// Created by hamzeh on 11/30/19.
//

#ifndef PX_SOFTWARE_SEARCHPACKAGESLIST_H
#define PX_SOFTWARE_SEARCHPACKAGESLIST_H
#include <QMovie>
#include <QBoxLayout>
#include <QLabel>

#include "PxQWidget.h"
#include "PackageListWidgetItem.h"
#include "PxQScrollArea.h"
#include "PackageManager.h"
#include "QProgressIndicator.h"

using namespace PKG;
class SearchPackagesList : public PxQScrollArea {
Q_OBJECT
public:
    enum SearchFilter{
        All,
        Latest,
        Installed,
        Upgradable,
    };
    SearchPackagesList(const QString &title, const SearchFilter &filter, PxQScrollArea *parent = nullptr)
            : PxQScrollArea(title, parent) {
        PackageManager *m_pkgMgr = PackageManager::Instance();
        this->filter = filter;
        connect(m_pkgMgr, SIGNAL(taskFailed(
                                         const QUuid &, const QString &)), this, SLOT(taskFailedHandler(
                                                                                              const QUuid &, const QString &)));
        connect(m_pkgMgr, SIGNAL(packageSearchResultsReady(
                                         const QUuid &, const QVector<Package *> &)), this,
                SLOT(packageSearchResultsReadyHandler(
                             const QUuid &, const QVector<Package *> &)));

        auto loading = new QProgressIndicator(this);
        loading->setFixedSize(VIEW_LOADING_ICON_SIZE,VIEW_LOADING_ICON_SIZE);
        loading->setStyleSheet("QLabel {background-color: transparent;}");
        loading->startAnimation();

        setAlignment(Qt::AlignCenter);
        setWidget(loading);
        taskId = m_pkgMgr->requestPackageSearch(title);
    };

    SearchFilter currentFilter(){
        return filter;
    }

private slots:

    void packageSearchResultsReadyHandler(const QUuid &taskId, const QVector<Package *> &packages) {
        boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        auto widget = new PxQWidget;
        widget->setLayout(boxLayout);
        setWidgetResizable(true);
        setWidget(widget);
        bool listIsEmpty = true;
        if(packages.size()){
            for (auto pkg:packages) {
                if(filter == SearchFilter::Upgradable){
                    if(pkg->isUpdateAvailable()){
                        auto packageWidget = new PackageListWidgetItem(pkg, true, this);
                        boxLayout->addWidget(packageWidget);
                        listIsEmpty=false;
                    }
                } else if(filter == SearchFilter::Installed){
                    if(pkg->isInstalled()){
                        auto packageWidget = new PackageListWidgetItem(pkg, true, this);
                        boxLayout->addWidget(packageWidget);
                        listIsEmpty=false;
                    }
                } else if(filter == SearchFilter::Latest){
                    for(const auto &t : pkg->tags()){
                        if( t == "latest" ){
                            auto packageWidget = new PackageListWidgetItem(pkg, false, this);
                            boxLayout->addWidget(packageWidget);
                            listIsEmpty=false;
                        }
                    }
                } else {
                    auto packageWidget = new PackageListWidgetItem(pkg, false, this);
                    boxLayout->addWidget(packageWidget);
                    listIsEmpty=false;
                }
            }
        }
        if(listIsEmpty){
            auto emptyLabel = new QLabel;
            emptyLabel->setText(tr("No record found for") + QString(" \"") + getTitle()+"\"");
            emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
            boxLayout->addWidget(emptyLabel);
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
            auto widget=new PxQWidget;
            widget->setLayout(boxLayout);
            setWidgetResizable(true);
            setWidget(widget);
        }
    }

private:
    QBoxLayout *boxLayout = nullptr;
    SearchFilter filter;
    QUuid taskId;
};


#endif //PX_SOFTWARE_SEARCHPACKAGESLIST_H
