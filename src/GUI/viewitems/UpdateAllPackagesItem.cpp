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
#include <QMovie>

#include "UpdateAllPackagesItem.h"
#include "Settings.h"

#define UPDATE_ALL_TITLE "UPDATE ALL"
#define UPDATING_TITLE   "Updating ..."
  
void UpdateAllPackagesItem_widget::refreshUpdateButtonStatus(){
    if(_isUpdating){
        _button->setText(tr(UPDATING_TITLE));
        _button->setStyleSheet(PACKAGE_INPROGRESS_STYLESHEET);
        _processLabel->setVisible(true);
        _movie->start();
    } else {
        _button->setText(tr(UPDATE_ALL_TITLE));
        _button->setStyleSheet(PACKAGE_UPDATE_STYLESHEET);
        _processLabel->setVisible(false);
        _movie->stop();
    }
}

void UpdateAllPackagesItem_widget::checkUserPackageList(const QString &pkgName){
    int i = 0;
    for(auto const &p :_updatingPackages){
        if(p == pkgName){
            _updatingPackages.remove(i);
        }
        i++;
    }
    if(_updatingPackages.size() == 0){
        _isUpdating = false;
        refreshUpdateButtonStatus();
    }
}

UpdateAllPackagesItem_widget::UpdateAllPackagesItem_widget(bool system, const QVector<Package *> &list, QWidget *parent) : 
    QWidget(parent),
    _systemPackages(system),
    _packageList(list),
    _pkgMgrTrk(PackageManagerTracker::Instance()) {

    connect(_pkgMgrTrk, &PackageManagerTracker::systemUpdateFinished,[&](const QString &outData, const QString &errData){
        if(_systemPackages && _isUpdating) {
            _isUpdating = false;
            refreshUpdateButtonStatus();
        }
    });

    connect(_pkgMgrTrk, SIGNAL(packageUpdated(const QString &)),this, SLOT(checkUserPackageList(const QString &)));
    connect(_pkgMgrTrk, SIGNAL(packageTaskCanceled(const QString &)),this, SLOT(checkUserPackageList(const QString &)));
    connect(_pkgMgrTrk, &PackageManagerTracker::userUpdatablePackageListReady,
                        [&](const QVector<Package *> &list){
                            _packageList = list;
                        });
        
    _button = new QPushButton(this);
    connect(_button,&QPushButton::pressed,[&](){
        if(_isUpdating){
            if(_systemPackages)
                _pkgMgrTrk->requestTaskCancel(_updatingAllTaskId);
            else {
                for(auto const &p: _updatingPackages){
                    _pkgMgrTrk->requestPackageTaskCancel(p);
                }
            }
            _isUpdating = false;
        } else {
            if(_systemPackages){
                _updatingAllTaskId = _pkgMgrTrk->requestSystemUpdate();
            } else {
                for(auto const &p: _packageList){
                    if(!_pkgMgrTrk->inUpdating(p->name())){
                        _updatingPackages.push_back(p->name());
                        _pkgMgrTrk->requestPackageUpdate(p->name());
                    }
                }
            }
            _isUpdating = true;
        }
        refreshUpdateButtonStatus();
    });
    _button->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
    _button->setContentsMargins(0,10,0,10);

    _movie = new QMovie(":images/general/src/GUI/resources/rolling-progress");
    _processLabel = new QLabel(this);
    _processLabel->setMovie(_movie);
    _processLabel->setAlignment(Qt::AlignCenter);
    _processLabel->setFixedSize(PACKAGE_BUTTON_W,IN_PROGRESS_GIF_HEIGHT);
    _movie->setScaledSize(_processLabel->size());
    
    // TODO read _isUpdating
    refreshUpdateButtonStatus();

    auto *lLayout = new QVBoxLayout();
    lLayout->addWidget(_button);
    lLayout->addWidget(_processLabel);
    lLayout->setAlignment(Qt::AlignCenter);

    QLabel *descLabel = new QLabel(this);
    if(_systemPackages){
        descLabel->setText(tr(UPDATE_ALL_SYS_PACKAGE_DESC));
    } else {
        descLabel->setText(tr(UPDATE_ALL_USER_PACKAGE_DESC));
    }
    auto descFont = descLabel->font();
    descFont.setPointSize(OTHER_PACKAGES_DESC_FONT_SIZE);
    descLabel->setFont(descFont);
    descLabel->setContentsMargins(10,20,20,20);
    descLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    descLabel->setStyleSheet("QLabel {background: transparent}");

    auto *layout = new QHBoxLayout();
    layout->addLayout(lLayout);
    layout->addWidget(descLabel);
    layout->setSpacing(10);
    layout->setMargin(0);
    setLayout(layout);
    setContentsMargins(20,20,20,20);
    setMouseTracking(false);
}
