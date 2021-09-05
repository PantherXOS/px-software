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
#define UPDATING_TITLE   "Updating..."
  
void UpdateAllPackagesItem_widget::refreshUpdateButtonStatus(){
    if(_isUpdating){
        _button->setFixedWidth(PACKAGE_BUTTON_INPROGRESS_W);
        _button->setText(tr(UPDATING_TITLE));
        if(!_systemPackages)
            _button->setDisabled(true);
        _cancelButton->setVisible(true);
        _button->setStyleSheet(PACKAGE_INPROGRESS_STYLESHEET);
        _processLabel->setVisible(true);
        _movie->start();
    } else {
        _button->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        _button->setText(tr(UPDATE_ALL_TITLE));
        if(!_systemPackages)
            _button->setDisabled(false);
        _cancelButton->setVisible(false);
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

void UpdateAllPackagesItem_widget::systemUpdateFinished(const QUuid &taskId, const QString &message){
    if(_systemPackages && _isUpdating && (taskId == _updatingAllTaskId)) {
        _isUpdating = false;
        _terminalMessage = "";
        refreshUpdateButtonStatus();
    }
}

UpdateAllPackagesItem_widget::UpdateAllPackagesItem_widget(bool system, const QVector<Package *> &list, QWidget *parent) : 
    QWidget(parent),
    _systemPackages(system),
    _packageList(list),
    _pkgMgrTrk(PackageManagerTracker::Instance()) {

    _connections.push_back(connect(_pkgMgrTrk, &PackageManagerTracker::taskDataReceivedWithUuid,[&](const QUuid &uuid, const QString &data){
        if(uuid == _updatingAllTaskId){
            _terminalMessage+=data;
            _terminalWidget->showMessage(_terminalMessage);
        }
    }));
    _connections.push_back(connect(_pkgMgrTrk, SIGNAL(packageUpdated(const QString &)),this, SLOT(checkUserPackageList(const QString &))));
    _connections.push_back(connect(_pkgMgrTrk, SIGNAL(packageTaskCanceled(const QString &)),this, SLOT(checkUserPackageList(const QString &))));
    _connections.push_back(connect(_pkgMgrTrk, &PackageManagerTracker::userUpdatablePackageListReady,
                        [&](const QVector<Package *> &list){
                            _packageList = list;
                        }));
    
    _cancelButton = new QPushButton(this);
    _cancelButton->setStyleSheet(PACKAGE_CANCEL_STYLESHEET);
    _cancelButton->setText("Cancel");
    _cancelButton->setVisible(false);
    _cancelButton->setFixedSize(PACKAGE_BUTTON_INPROGRESS_W,PACKAGE_BUTTON_H);
    _connections.push_back(connect(_cancelButton,&QPushButton::pressed,[&](){
        if(_isUpdating){
            if(_systemPackages){
                _pkgMgrTrk->requestTaskCancel(_updatingAllTaskId);
                _terminalMessage = "";
            } else {
                for(auto const &p: _updatingPackages)
                    _pkgMgrTrk->requestPackageTaskCancel(p);
            }
            _isUpdating = false;
        }
        refreshUpdateButtonStatus();
    }));
    
    _button = new QPushButton(this);
    _connections.push_back(connect(_button,&QPushButton::pressed,[&](){
        if(_isUpdating){
            emit showTerminalSignal(_terminalWidget);
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
            refreshUpdateButtonStatus();
        }
    }));
    _button->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
    if(_systemPackages){
        _connections.push_back(connect(_pkgMgrTrk, SIGNAL(taskDone(const QUuid &, const QString &)),this, SLOT(systemUpdateFinished(const QUuid &, const QString &))));
        _connections.push_back(connect(_pkgMgrTrk, SIGNAL(taskFailed(const QUuid &, const QString &)),this, SLOT(systemUpdateFinished(const QUuid &, const QString &))));
        _terminalWidget = new TerminalWidget("SYSTEM Update");
        _button->installEventFilter(this);
        _button->setObjectName("update_all_system");
    }

    _movie = new QMovie(":images/general/src/GUI/resources/rolling-progress");
    _processLabel = new QLabel(this);
    _processLabel->setMovie(_movie);
    _processLabel->setAlignment(Qt::AlignCenter);
    _processLabel->setFixedSize(PACKAGE_BUTTON_W,IN_PROGRESS_GIF_HEIGHT);
    _movie->setScaledSize(_processLabel->size());

    auto *tLayout = new QHBoxLayout();
    tLayout->addWidget(_button);
    tLayout->addWidget(_cancelButton);
    tLayout->setAlignment(Qt::AlignCenter);
    
    auto *lLayout = new QVBoxLayout();
    lLayout->addLayout(tLayout);
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
    descLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    auto *layout = new QHBoxLayout();
    layout->addLayout(lLayout);
    layout->addWidget(descLabel);
    layout->setContentsMargins(20,0,20,0);
    setLayout(layout);
    setMouseTracking(false);

    refreshUpdateButtonStatus();
    setFixedHeight(UPDATE_ALL_ITEM_SIZE_H);
    setStyleSheet("QWidget {background: " UPDATE_ITEM_BACKGROUND_COLOR "}");
    setAttribute(Qt::WA_StyledBackground, true);
}

bool UpdateAllPackagesItem_widget::eventFilter(QObject* object, QEvent* event) {
    if(event->type() == QEvent::HoverEnter) {
        if (((QPushButton*)object)->text() == UPDATING_TITLE)
            ((QPushButton*)object)->setText("Status");
    } else if(event->type() == QEvent::HoverLeave) {
        if (((QPushButton*)object)->text() == "Status")
            ((QPushButton*)object)->setText(UPDATING_TITLE);
    }
    return QWidget::eventFilter(object, event);
}

UpdateAllPackagesItem_widget::~UpdateAllPackagesItem_widget(){
    for(auto const &c: _connections){
        disconnect(c);
    }
}