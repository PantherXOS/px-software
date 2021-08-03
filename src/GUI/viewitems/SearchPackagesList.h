/*
 * px-software - Qt-GUI package manager for guix on PantherX OS
 * Copyright (C) 2019, 2020  Hamzeh Nasajpour <h.nasajpour@pantherx.org>
 * Copyright (C) 2019, 2020  Reza Alizadeh Majd <r.majd@pantherx.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef PX_SOFTWARE_SEARCHPACKAGESLIST_H
#define PX_SOFTWARE_SEARCHPACKAGESLIST_H
#include <QMovie>
#include <QBoxLayout>
#include <QLabel>

#include <QWidget>
#include "PackageListWidget.h"
#include "OtherApplicationsWidgetItem.h"

using namespace PKG;
class SearchPackagesList : public PackageListWidget {
Q_OBJECT
public:
    enum SearchFilter{
        All,
        Latest,
        Installed,
        Upgradable,
    };
    SearchPackagesList(const QString &title, const SearchFilter &filter, PXContentWidget *parent = nullptr)
            : PackageListWidget(title, parent) {
        this->filter = filter;
        connect(m_pkgMgr, SIGNAL(packageSearchResultsReady(const QUuid &, const QVector<Package *> &)), this,
                          SLOT(packageSearchResultsReadyHandler(const QUuid &, const QVector<Package *> &)));
        taskId = m_pkgMgr->requestPackageSearch(title);
    };

    SearchFilter currentFilter(){
        return filter;
    }

private slots:

    void packageSearchResultsReadyHandler(const QUuid &taskId, const QVector<Package *> &packages) {
        // TODO review
        // if(filter == SearchFilter::Upgradable || filter == SearchFilter::Installed)
        setLoadingVisible(false);
        setListVisible(true);
        QVector<Package *> otherPackageList;
        for(auto &pkg:packages) {
            bool removeEnable = false;
            if(pkg->isAvailableInDB()) {
                if(pkg->isUpdateAvailable() || pkg->isInstalled())
                    removeEnable = true;
                auto packageWidget = new PackageListWidgetItem1(pkg, true, removeEnable, this);
                addItem(packageWidget);
            } else {
                otherPackageList.append(pkg);
            }
        }
        
        if(otherPackageList.size()) {
            auto otherApplicationTitle = new OtherApplicationsWidgetItem1(this);
            addItem(otherApplicationTitle);
        }
        
        for(auto &pkg:otherPackageList){
            bool removeEnable = false;
            if(pkg->isUpdateAvailable() || pkg->isInstalled())
                removeEnable = true;
            auto packageWidget = new PackageListWidgetItem1(pkg, true, removeEnable, this);
            addItem(packageWidget);
        }

        if(!packages.size()){
            auto emptyLabel = new QLabel;
            emptyLabel->setText(tr("No record found for") + QString(" \"") + title()+"\"");
            emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
            addItem(emptyLabel);
        }
    }

    void taskFailedHandler(const QUuid &_taskId, const QString &message) override{
        if(_taskId == taskId){
            setLoadingVisible(false);
            setListVisible(true);
            auto emptyLabel = new QLabel;
            emptyLabel->setText(message);
            emptyLabel->setFont(QFont("default", VIEW_MESSAGE_FONT_SIZE));
            addItem(emptyLabel);
        }
    }

private:
    SearchFilter filter;
    QUuid taskId;
};


#endif //PX_SOFTWARE_SEARCHPACKAGESLIST_H
