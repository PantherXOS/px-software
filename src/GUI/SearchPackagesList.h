//
// Created by hamzeh on 11/30/19.
//

#ifndef PX_SOFTWARE_SEARCHPACKAGESLIST_H
#define PX_SOFTWARE_SEARCHPACKAGESLIST_H
#include <QMovie>
#include <QBoxLayout>
#include <PackageListWidgetItem.h>

#include "PxQScrollArea.h"
#include "PackageManager.h"
#include "QLabel"

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
        m_pkgMgr->requestPackageSearch(title);

        QMovie *movie = new QMovie(":images/general/src/GUI/resources/loading.gif");
        QSize size(128, 128);
        movie->setScaledSize(size);
        setAlignment(Qt::AlignCenter);
        QLabel *processLabel = new QLabel(this);
        processLabel->setMovie(movie);
        processLabel->setFixedSize(size);
        movie->start();
        setWidget(processLabel);
    };

private slots:

    void packageSearchResultsReadyHandler(const QUuid &taskId, const QVector<Package *> &packages) {
        if (boxLayout != nullptr)
            delete boxLayout;

        boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        QWidget *widget = new QWidget;
        widget->setLayout(boxLayout);
        setWidgetResizable(true);
        setWidget(widget);
        for (auto pkg:packages) {
            if(filter == SearchFilter::Upgradable){
                if(pkg->isUpdateAvailable()){
                    auto packageWidget = new PackageListWidgetItem(pkg, true, this);
                    boxLayout->addWidget(packageWidget);
                }
            } else if(filter == SearchFilter::Installed){
                if(pkg->isInstalled()){
                    auto packageWidget = new PackageListWidgetItem(pkg, true, this);
                    boxLayout->addWidget(packageWidget);
                }
            } else if(filter == SearchFilter::Latest){
                for(const auto &t : pkg->tags()){
                    if( t == "latest" ){
                        auto packageWidget = new PackageListWidgetItem(pkg, false, this);
                        boxLayout->addWidget(packageWidget);
                    }
                }
            } else {
                auto packageWidget = new PackageListWidgetItem(pkg, false, this);
                boxLayout->addWidget(packageWidget);
            }
        }
    }

    void taskFailedHandler(const QUuid &taskId, const QString &message) {
        qDebug() << this << " : " << message;
    }

private:
    QBoxLayout *boxLayout = nullptr;
    SearchFilter filter;
};


#endif //PX_SOFTWARE_SEARCHPACKAGESLIST_H
