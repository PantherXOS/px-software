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

#ifndef PX_SOFTWARE_PACKAGECOMPONENT_H
#define PX_SOFTWARE_PACKAGECOMPONENT_H
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <QMovie>
#include <QGuiApplication>

#include "DownloadManager.h"
#include "PackageManagerTracker.h"
#include "PackageManager.h"
#include "PXSeperator.h"
#include "TerminalWidget.h"
#include "Settings.h"
#include "CacheManager.h"
#include "src/GUI/notification/pxnotification.h"

class PackageComponent : public QWidget{
    Q_OBJECT
public:
    PackageComponent(Package *package, QWidget *parent = nullptr) : QWidget(parent), m_package(package){
        this->allButtonEnable = true;
        init();
    }

    PackageComponent(Package *package, bool updateEnable, bool removeEnable, QWidget *parent = nullptr) : QWidget(parent), m_package(package){
        this->removeButtonEnable = removeEnable;
        this->updateButtonEnable = updateEnable;
        init();
    }

    QHBoxLayout *getIconLayout(){
        return iconLayout;
    }

    QVBoxLayout *getButtonsLayoutAsList(){
        auto buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(_updateButton);
        buttonLayout->addWidget(_removeButton);
        buttonLayout->addWidget(_installButton);
        buttonLayout->addWidget(_upToDateButton);
        buttonLayout->addWidget(_cancelButton);
        buttonLayout->setAlignment(Qt::AlignCenter);
        buttonLayout->setContentsMargins(0,10,0,10);

        auto layout = new QVBoxLayout;
        layout->addLayout(buttonLayout);
        layout->addWidget(processLabel);
        layout->setAlignment(Qt::AlignCenter);
        return layout;
    }

    QVBoxLayout *getButtonsLayoutAsDetails(){
        auto line = new PXSeperator(this);

        QLabel *version = new QLabel(tr("Version") + " : " + m_package->version(),this);
        version->setFixedWidth(PACKAGE_DETAILS_LEFT_PANEL_SIZE);
        version->setWordWrap(true);

        QLabel *license = new QLabel(tr("License") + " : " + m_package->license(),this);
        license->setFixedWidth(PACKAGE_DETAILS_LEFT_PANEL_SIZE);
        license->setWordWrap(true);
        
        auto buttonProgressLayout = new QHBoxLayout;
        buttonProgressLayout->addWidget(_updateButton);
        buttonProgressLayout->addWidget(_removeButton);
        buttonProgressLayout->addWidget(_installButton);
        buttonProgressLayout->addWidget(_cancelButton);
        auto buttonLayout = new QVBoxLayout;
        buttonLayout->addLayout(buttonProgressLayout);
        buttonLayout->addWidget(processLabel);
        buttonLayout->addWidget(line);
        buttonLayout->addWidget(version);
        buttonLayout->addWidget(license);
        buttonLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        return buttonLayout;
    }

private slots:
    void imageDownloaded(const FileDownloader::DownloadItem &item){
        if(downloaderId == item.uuid){
            QIcon qicon;
            QImage image(item.localFilePath + item.localFileName);
            qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
            QPixmap pixmap = qicon.pixmap(QSize(PACKAGE_ICON_SIZE, PACKAGE_ICON_SIZE), QIcon::Normal, QIcon::On);
            iconButton->setPixmap(pixmap);
        }
    };

    void cancelButtonHandler(){
        m_pkgMgrTrk->requestPackageTaskCancel(m_package->name());
        reloadButtonsStatus();
    }

    void installButtonHandler(){
        if(m_pkgMgrTrk->requestPackageInstallation(m_package->name())) {
            reloadButtonsStatus();
        } else {
            if(!this->terminal)
                this->terminal = new TerminalWidget(m_package->name());
            emit showTerminalSignal(this->terminal);
        }
    }

    void removeButtonHandler(){
        if(m_pkgMgrTrk->requestPackageRemoval(m_package->name())) {
            reloadButtonsStatus();
        } else {
            if(!this->terminal)
                this->terminal = new TerminalWidget(m_package->name());
            emit showTerminalSignal(this->terminal);
        }
    }

    void updateButtonHandler(){
        if(m_pkgMgrTrk->requestPackageUpdate(m_package->name())) {
            reloadButtonsStatus();
        } else {
            if(!this->terminal)
                this->terminal = new TerminalWidget(m_package->name());    
            emit showTerminalSignal(this->terminal);
        }
    }

    void taskFailedHandler(const QString &name, const QString &message){
        if(name == m_package->name()){
            disconnect(failedProgressConnection);
            reloadButtonsStatus();
        }
    }

    void taskDataReceivedHandler(const QString & name, const QString &message) {
        if(this->m_package->name() == name){
            debugMessage+=message;
            QStringList lines = debugMessage.split('\r');
            debugMessage = lines.at(0);
            if(lines.size()>2)
                debugMessage+=lines.at(lines.size()-1);
            if(this->terminal != nullptr)
                this->terminal->showMessage(debugMessage);
        }
    }

    void packageUpdatedHandler(const QString &name){
        if(name == m_package->name()){
            this->m_package->setUpdateAvailable(false);
            Notification::notify(name + tr(" updating finished."));
            reloadButtonsStatus();
        }
    }

    void packageRemovedHandler(const QString &name){
        if(name == m_package->name()){
            this->m_package->setInstalled(false);
            Notification::notify(name + tr(" removal finished."));
            reloadButtonsStatus();
        }
    }

    void packageInstalledHandler(const QString &name){
        if(name == m_package->name()){
            this->m_package->setInstalled(true);
            Notification::notify(name + tr(" installation finished."));
            reloadButtonsStatus();
        }
    }

    void taskCanceledHandler(const QString &name) {
        if(name == m_package->name()) {
            reloadButtonsStatus();
        }
    }

    void reloadButtonsStatus(){
        movie->stop();
        processLabel->setVisible(false);
        _cancelButton->setVisible(false);
        _updateButton->setVisible(false);
        _removeButton->setVisible(false);
        _upToDateButton->setVisible(false);
        _installButton->setVisible(false);
        _installButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        _removeButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        _updateButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        if(m_pkgMgrTrk->inInstalling(m_package->name())) {
            _cancelButton->setVisible(true);
            processLabel->setVisible(true);
            movie->start();
            
            _installButton->setText(tr(PACKAGE_BUTTON_INSTALLING));
            _installButton->setStyleSheet(PACKAGE_INPROGRESS_STYLESHEET);
            _installButton->setVisible(true);
            _installButton->setFixedSize(PACKAGE_BUTTON_INPROGRESS_W,PACKAGE_BUTTON_H);
        } else if(m_pkgMgrTrk->inRemoving(m_package->name())) {
            _cancelButton->setVisible(true);
            processLabel->setVisible(true);
            movie->start();

            _removeButton->setText(tr(PACKAGE_BUTTON_REMOVING));
            _removeButton->setStyleSheet(PACKAGE_INPROGRESS_STYLESHEET);
            _removeButton->setVisible(true);
            _removeButton->setFixedSize(PACKAGE_BUTTON_INPROGRESS_W,PACKAGE_BUTTON_H);
        } else if(updatingAll || m_pkgMgrTrk->inUpdating(m_package->name())) {
            _cancelButton->setVisible(true);
            processLabel->setVisible(true);
            movie->start();

            _updateButton->setText(tr(PACKAGE_BUTTON_UPDATING));
            _updateButton->setStyleSheet(PACKAGE_INPROGRESS_STYLESHEET);
            _updateButton->setVisible(true);
            _updateButton->setFixedSize(PACKAGE_BUTTON_INPROGRESS_W,PACKAGE_BUTTON_H);
        } else {
            if(allButtonEnable){
                if(m_package->isInstalled()){
                    if(m_package->isUpdateAvailable()){
                        _updateButton->setText(updateButtonTitle);
                        _updateButton->setStyleSheet(PACKAGE_UPDATE_STYLESHEET);
                        _updateButton->setVisible(true);
                    }
                    _removeButton->setText(tr("Remove"));
                    _removeButton->setStyleSheet(PACKAGE_REMOVE_STYLESHEET);
                    _removeButton->setVisible(true);
                } else {
                    _installButton->setText(tr("Install"));
                    _installButton->setStyleSheet(PACKAGE_INSTALL_STYLESHEET);
                    _installButton->setVisible(true);
                }
            } else {
                if(m_package->isInstalled()) {
                    if (m_package->isUpdateAvailable()) {
                        _updateButton->setText(updateButtonTitle);
                        _updateButton->setStyleSheet(PACKAGE_UPDATE_STYLESHEET);
                        if(updateButtonEnable) 
                            _updateButton->setVisible(true);
                    } else if(removeButtonEnable) {
                        _removeButton->setText(tr("Remove"));
                        _removeButton->setStyleSheet(PACKAGE_REMOVE_STYLESHEET);
                        _removeButton->setVisible(true);
                    } else {
                        _upToDateButton->setText(tr("Up to date"));
                        _upToDateButton->setStyleSheet(PACKAGE_BUTTON_DISABLE_STYLESHEET);
                        if(removeButtonEnable || updateButtonEnable) 
                            _upToDateButton->setVisible(true);
                    }
                } else {
                    _installButton->setText(tr("Install"));
                    _installButton->setStyleSheet(PACKAGE_INSTALL_STYLESHEET);
                    _installButton->setVisible(true);
                }
            }
        }
    }

signals:
    void showTerminalSignal(TerminalWidget *terminal);

private:
    bool eventFilter(QObject* object, QEvent* event) {
        if(event->type() == QEvent::HoverEnter) {
            auto buttonText = ((QPushButton*)object)->text();
            if(buttonText == PACKAGE_BUTTON_INSTALLING || buttonText == PACKAGE_BUTTON_REMOVING || buttonText == PACKAGE_BUTTON_UPDATING )
                ((QPushButton*)object)->setText("Status");
        } else if(event->type() == QEvent::HoverLeave) {
            auto buttonObjectName = ((QPushButton*)object)->objectName();
            auto buttonText = ((QPushButton*)object)->text();
            if(buttonText == "Status"){
                if(buttonObjectName == PACKAGE_BUTTON_OBJ_NAME_INSTALL)
                    ((QPushButton*)object)->setText(PACKAGE_BUTTON_INSTALLING);
                else if(buttonObjectName == PACKAGE_BUTTON_OBJ_NAME_REMOVE)
                    ((QPushButton*)object)->setText(PACKAGE_BUTTON_REMOVING);
                else if(buttonObjectName == PACKAGE_BUTTON_OBJ_NAME_UPDATE)
                    ((QPushButton*)object)->setText(PACKAGE_BUTTON_UPDATING);
            }
        }
        return QWidget::eventFilter(object, event);
    }
    
    void init(){
        m_pkgMgrTrk = PackageManagerTracker::Instance();
        connect(m_pkgMgrTrk, SIGNAL(taskDataReceived(const QString&,const QString&)),this, SLOT(taskDataReceivedHandler(const QString,const QString&)));
        connect(m_pkgMgrTrk, SIGNAL(packageUpdated(const QString)),this, SLOT(packageUpdatedHandler(const QString)));
        connect(m_pkgMgrTrk, SIGNAL(packageRemoved(const QString)),this, SLOT(packageRemovedHandler(const QString)));
        connect(m_pkgMgrTrk, SIGNAL(packageInstalled(const QString)),this, SLOT(packageInstalledHandler(const QString)));
        connect(m_pkgMgrTrk, SIGNAL(packageTaskCanceled(const QString)),this, SLOT(taskCanceledHandler(const QString)));
        connect(m_pkgMgrTrk, SIGNAL(inProgressRequest()),this, SLOT(reloadButtonsStatus()));
        failedProgressConnection  = connect(m_pkgMgrTrk, SIGNAL(progressFailed(const QString&,const QString&)),this, SLOT(taskFailedHandler(const QString,const QString&)));

        createButtonsLayout();
        createIconLayout(m_package->icon());
    }
    
    void createIconLayout(const QUrl &iconUrl){
        iconButton = new QLabel(this);
        iconButton->setFixedSize(QSize(PACKAGE_ICON_SIZE, PACKAGE_ICON_SIZE));
        iconButton->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        iconButton->setStyleSheet(PACKAGE_ICON_STYLESHEET);
        iconLayout = new QHBoxLayout;
        iconLayout->addWidget(iconButton);

        QString iconFileLocalPath = CacheManager::instance()->cacheDir()+PACKAGE_ICON_CACHE_DIR + QString(this->m_package->name()) + QString("/");
        FileDownloader::DownloadItem iconItem(iconUrl,iconFileLocalPath, iconUrl.fileName());
        QFile iconFile(iconItem.localFilePath + iconItem.localFileName);
        if(!iconFile.exists()){
            m_pImgCtrl = DownloadManager::Instance();
            connect(m_pImgCtrl, SIGNAL (downloadComplete(const FileDownloader::DownloadItem&)), this, SLOT (imageDownloaded(const FileDownloader::DownloadItem&)));
            downloaderId = m_pImgCtrl->download(iconItem);
        } else {
            downloaderId = iconItem.uuid = QUuid::createUuid();
            imageDownloaded(iconItem);
        }
        reloadButtonsStatus();
    }

    void createButtonsLayout(){
        // add install,update and remove buttons
        movie = new QMovie(":images/general/src/GUI/resources/rolling-progress");
        processLabel = new QLabel(this);
        processLabel->setMovie(movie);
        processLabel->setAlignment(Qt::AlignCenter);
        processLabel->setFixedSize(PACKAGE_BUTTON_W,IN_PROGRESS_GIF_HEIGHT);
        movie->setScaledSize(processLabel->size());

        _cancelButton = new QPushButton(this);
        _cancelButton->setText(tr("Cancel"));
        _cancelButton->setFixedSize(PACKAGE_BUTTON_INPROGRESS_W,PACKAGE_BUTTON_H);
        _cancelButton->setStyleSheet(PACKAGE_CANCEL_STYLESHEET);
        connect(_cancelButton, SIGNAL(released()), this, SLOT(cancelButtonHandler()));

        _updateButton = new QPushButton(this);
        _updateButton->setText(updateButtonTitle);
        _updateButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        _updateButton->setStyleSheet(PACKAGE_UPDATE_STYLESHEET);
        _updateButton->setObjectName(PACKAGE_BUTTON_OBJ_NAME_UPDATE);
        _updateButton->installEventFilter(this);
        connect(_updateButton, SIGNAL(released()), this, SLOT(updateButtonHandler()));

        _removeButton = new QPushButton(this);
        _removeButton->setText(tr("Remove"));
        _removeButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        _removeButton->setStyleSheet(PACKAGE_REMOVE_STYLESHEET);
        _removeButton->setObjectName(PACKAGE_BUTTON_OBJ_NAME_REMOVE);
        _removeButton->installEventFilter(this);
        connect(_removeButton, SIGNAL(released()), this, SLOT(removeButtonHandler()));

        _installButton = new QPushButton(this);
        _installButton->setText(tr("Install"));
        _installButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        _installButton->setStyleSheet(PACKAGE_INSTALL_STYLESHEET);
        _installButton->setObjectName(PACKAGE_BUTTON_OBJ_NAME_INSTALL);
        _installButton->installEventFilter(this);
        connect(_installButton, SIGNAL(released()), this, SLOT(installButtonHandler()));

        _upToDateButton = new QPushButton(this);
        _upToDateButton->setText(tr("Up to date"));
        _upToDateButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        _upToDateButton->setStyleSheet(PACKAGE_BUTTON_DISABLE_STYLESHEET);

        reloadButtonsStatus();
    }

    QMovie *movie;
    QLabel *processLabel;
    bool removeButtonEnable = false;
    bool updateButtonEnable = false;
    bool allButtonEnable = false;
    bool updatingAll = false;
    QMetaObject::Connection failedProgressConnection;
    QPushButton *_updateButton, *_removeButton, *_installButton, *_upToDateButton, *_cancelButton;
    QLabel *iconButton;
    QHBoxLayout *iconLayout;
    QString debugMessage;
    DownloadManager *m_pImgCtrl;
    Package *m_package;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
    TerminalWidget *terminal = nullptr;
    QString updateButtonTitle = tr("Update");
    QUuid   downloaderId;
};


#endif //PX_SOFTWARE_PACKAGECOMPONENT_H
