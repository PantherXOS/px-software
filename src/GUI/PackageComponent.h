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

#include "FileDownloader.h"
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
    PackageComponent(Package *package, QWidget *parent = nullptr) : QWidget(parent){
        this->package = package;
        m_pkgMgrTrk = PackageManagerTracker::Instance();
        connect(m_pkgMgrTrk, SIGNAL(taskDataReceived(const QString&,const QString&)),this, SLOT(taskDataReceivedHandler(const QString,const QString&)));
        connect(m_pkgMgrTrk, SIGNAL(packageUpdated(const QString)),this, SLOT(packageUpdatedHandler(const QString)));
        connect(m_pkgMgrTrk, SIGNAL(packageRemoved(const QString)),this, SLOT(packageRemovedHandler(const QString)));
        connect(m_pkgMgrTrk, SIGNAL(packageInstalled(const QString)),this, SLOT(packageInstalledHandler(const QString)));
        connect(m_pkgMgrTrk, SIGNAL(packageTaskCanceled(const QString)),this, SLOT(taskCanceledHandler(const QString)));
        connect(m_pkgMgrTrk, SIGNAL(inProgressRequest()),this, SLOT(reloadButtonsStatus()));
        failedProgressConnection  = connect(m_pkgMgrTrk, SIGNAL(progressFailed(const QString&,const QString&)),this, SLOT(taskFailedHandler(const QString,const QString&)));

        this->allButtonEnable = true;
        createButtonsLayout();
        createIconLayout(package->icon());
        this->terminal = new TerminalWidget(package->name());
    }

    PackageComponent(Package *package, bool removeEnable, QWidget *parent = nullptr) : QWidget(parent){
        this->package = package;
        m_pkgMgrTrk = PackageManagerTracker::Instance();
        connect(m_pkgMgrTrk, SIGNAL(taskDataReceived(const QString&,const QString&)),this, SLOT(taskDataReceivedHandler(const QString,const QString&)));
        connect(m_pkgMgrTrk, SIGNAL(packageUpdated(const QString)),this, SLOT(packageUpdatedHandler(const QString)));
        connect(m_pkgMgrTrk, SIGNAL(packageRemoved(const QString)),this, SLOT(packageRemovedHandler(const QString)));
        connect(m_pkgMgrTrk, SIGNAL(packageInstalled(const QString)),this, SLOT(packageInstalledHandler(const QString)));
        connect(m_pkgMgrTrk, SIGNAL(packageTaskCanceled(const QString)),this, SLOT(taskCanceledHandler(const QString)));
        connect(m_pkgMgrTrk, SIGNAL(inProgressRequest()),this, SLOT(reloadButtonsStatus()));
        failedProgressConnection  = connect(m_pkgMgrTrk, SIGNAL(progressFailed(const QString&,const QString&)),this, SLOT(taskFailedHandler(const QString,const QString&)));

        this->removeButtonEnable = removeEnable;
        createButtonsLayout();
        createIconLayout(package->icon());
        this->terminal = new TerminalWidget(package->name());
    }

    QHBoxLayout *getIconLayout(){
        return iconLayout;
    }

    QVBoxLayout *getButtonsLayoutAsList(){
        auto buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(updateButton);
        buttonLayout->addWidget(removeButton);
        buttonLayout->addWidget(installButton);
        buttonLayout->addWidget(upToDateButton);
        buttonLayout->addWidget(cancelButton);
        buttonLayout->setAlignment(Qt::AlignCenter);

        auto layout = new QVBoxLayout;
        layout->addLayout(buttonLayout);
        layout->addWidget(processLabel);
        layout->setAlignment(Qt::AlignCenter);
        return layout;
    }

    QVBoxLayout *getButtonsLayoutAsDetails(){
        auto line = new PXSeperator(this);

        auto pal = QGuiApplication::palette();
        auto bgcolor = pal.color(QPalette::Active, QPalette::Base);
        auto fgcolor = pal.color(QPalette::Active, QPalette::Text);
        QLabel *version = new QLabel(tr("Version") + " : " + package->version(),this);
        version->setStyleSheet(QString(QLABEL_STYLE_FROM_COLOR_SCHEME).arg(bgcolor.name(),fgcolor.name()));
        version->setFixedWidth(PACKAGE_DETAILS_LEFT_PANEL_SIZE);
        version->setWordWrap(true);

        QLabel *license = new QLabel(tr("License") + " : " + package->license(),this);
        license->setStyleSheet(QString(QLABEL_STYLE_FROM_COLOR_SCHEME).arg(bgcolor.name(),fgcolor.name()));
        license->setFixedWidth(PACKAGE_DETAILS_LEFT_PANEL_SIZE);
        license->setWordWrap(true);
        
        auto buttonProgressLayout = new QHBoxLayout;
        buttonProgressLayout->addWidget(updateButton);
        buttonProgressLayout->addWidget(removeButton);
        buttonProgressLayout->addWidget(installButton);
        buttonProgressLayout->addWidget(cancelButton);
        auto buttonLayout = new QVBoxLayout;
        buttonLayout->addLayout(buttonProgressLayout);
        buttonLayout->addWidget(processLabel);
        buttonLayout->addWidget(line);
        buttonLayout->addWidget(version);
        buttonLayout->addWidget(license);
        buttonLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        return buttonLayout;
    }

    TerminalWidget * getTerminal() {
        this->terminal->showMessage(debugMessage);
        return this->terminal;
    }

private slots:
    void imageDownloaded(const QString & localfile){
        QIcon qicon;
        QImage image(localfile);
        qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
        QPixmap pixmap = qicon.pixmap(QSize(PACKAGE_ICON_SIZE, PACKAGE_ICON_SIZE), QIcon::Normal, QIcon::On);
        iconButton->setPixmap(pixmap);
    };

    void cancelButtonHandler(){
        m_pkgMgrTrk->requestPackageTaskCancel(package->name());
        reloadButtonsStatus();
    }

    void installButtonHandler(){
        if(m_pkgMgrTrk->requestPackageInstallation(package->name())) {
            reloadButtonsStatus();
        } else {
            emit showTerminalSignal(this->terminal);
        }
    }

    void removeButtonHandler(){
        if(m_pkgMgrTrk->requestPackageRemoval(package->name())) {
            reloadButtonsStatus();
        } else {
            emit showTerminalSignal(this->terminal);
        }
    }

    void updateButtonHandler(){
        if(m_pkgMgrTrk->requestPackageUpdate(package->name())) {
            reloadButtonsStatus();
        } else {
            emit showTerminalSignal(this->terminal);
        }
    }

    void taskFailedHandler(const QString &name, const QString &message){
        if(name == package->name()){
            disconnect(failedProgressConnection);
            reloadButtonsStatus();
        }
    }

    void taskDataReceivedHandler(const QString & name, const QString &message) {
        if(this->package->name() == name){
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
        if(name == package->name()){
            this->package->setUpdateAvailable(false);
            Notification::notify(name + tr(" updating finished."));
            reloadButtonsStatus();
        }
    }

    void packageRemovedHandler(const QString &name){
        if(name == package->name()){
            this->package->setInstalled(false);
            Notification::notify(name + tr(" removal finished."));
            reloadButtonsStatus();
        }
    }

    void packageInstalledHandler(const QString &name){
        if(name == package->name()){
            this->package->setInstalled(true);
            Notification::notify(name + tr(" installation finished."));
            reloadButtonsStatus();
        }
    }

    void taskCanceledHandler(const QString &name) {
        if(name == package->name()) {
            reloadButtonsStatus();
        }
    }

    void reloadButtonsStatus(){
        movie->stop();
        processLabel->setVisible(false);
        cancelButton->setVisible(false);
        updateButton->setVisible(false);
        removeButton->setVisible(false);
        upToDateButton->setVisible(false);
        installButton->setVisible(false);
        installButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        removeButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        updateButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        if(m_pkgMgrTrk->inInstalling(package->name())) {
            cancelButton->setVisible(true);
            processLabel->setVisible(true);
            movie->start();
            
            installButton->setText(tr("Installing"));
            installButton->setStyleSheet(PACKAGE_INPROGRESS_STYLESHEET);
            installButton->setVisible(true);
            installButton->setFixedSize(PACKAGE_BUTTON_INPROGRESS_W,PACKAGE_BUTTON_H);
        } else if(m_pkgMgrTrk->inRemoving(package->name())) {
            cancelButton->setVisible(true);
            processLabel->setVisible(true);
            movie->start();

            removeButton->setText(tr("Removing"));
            removeButton->setStyleSheet(PACKAGE_INPROGRESS_STYLESHEET);
            removeButton->setVisible(true);
            removeButton->setFixedSize(PACKAGE_BUTTON_INPROGRESS_W,PACKAGE_BUTTON_H);
        } else if(m_pkgMgrTrk->inUpdating(package->name())) {
            cancelButton->setVisible(true);
            processLabel->setVisible(true);
            movie->start();

            updateButton->setText(tr("Updating"));
            updateButton->setStyleSheet(PACKAGE_INPROGRESS_STYLESHEET);
            updateButton->setVisible(true);
            updateButton->setFixedSize(PACKAGE_BUTTON_INPROGRESS_W,PACKAGE_BUTTON_H);
        } else {
            if(allButtonEnable){
                if(package->isInstalled()){
                    if(package->isUpdateAvailable()){
                        updateButton->setText(tr("Update"));
                        updateButton->setStyleSheet(PACKAGE_UPDATE_STYLESHEET);
                        updateButton->setVisible(true);
                    }
                    removeButton->setText(tr("Remove"));
                    removeButton->setStyleSheet(PACKAGE_REMOVE_STYLESHEET);
                    removeButton->setVisible(true);
                } else {
                    installButton->setText(tr("Install"));
                    installButton->setStyleSheet(PACKAGE_INSTALL_STYLESHEET);
                    installButton->setVisible(true);
                }
            } else {
                if(package->isInstalled()) {
                    if (package->isUpdateAvailable()) {
                        updateButton->setText(tr("Update"));
                        updateButton->setStyleSheet(PACKAGE_UPDATE_STYLESHEET);
                        updateButton->setVisible(true);
                    } else if(removeButtonEnable) {
                        removeButton->setText(tr("Remove"));
                        removeButton->setStyleSheet(PACKAGE_REMOVE_STYLESHEET);
                        removeButton->setVisible(true);
                    } else {
                        upToDateButton->setText(tr("Up to date"));
                        upToDateButton->setStyleSheet(PACKAGE_UPTODATE_STYLESHEET);
                        upToDateButton->setVisible(true);
                    }
                } else {
                    installButton->setText(tr("Install"));
                    installButton->setStyleSheet(PACKAGE_INSTALL_STYLESHEET);
                    installButton->setVisible(true);
                }
            }
        }
    }

signals:
    void showTerminalSignal(TerminalWidget *terminal);

private:
    void createIconLayout(const QUrl &iconUrl){
        iconButton = new QLabel(this);
        iconButton->setFixedSize(QSize(PACKAGE_ICON_SIZE, PACKAGE_ICON_SIZE));
        iconButton->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        iconButton->setStyleSheet(PACKAGE_ICON_STYLESHEET);

        iconLayout = new QHBoxLayout;
        iconLayout->addWidget(iconButton);

        QString iconFileLocalPath = CacheManager::instance()->cacheDir()+PACKAGE_ICON_CACHE_DIR + QString(this->package->name()) + QString("/");
        QString iconFilePath = iconFileLocalPath+iconUrl.fileName();
        m_pImgCtrl = new FileDownloader(this);
        connect(m_pImgCtrl, SIGNAL (downloaded(const QString &)), this, SLOT (imageDownloaded(const QString &)));
        m_pImgCtrl->start(iconUrl, iconFileLocalPath);
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

        cancelButton = new QPushButton(this);
        cancelButton->setText(tr("Cancel"));
        cancelButton->setFixedSize(PACKAGE_BUTTON_INPROGRESS_W,PACKAGE_BUTTON_H);
        cancelButton->setStyleSheet(PACKAGE_CANCEL_STYLESHEET);
        connect(cancelButton, SIGNAL(released()), this, SLOT(cancelButtonHandler()));

        updateButton = new QPushButton(this);
        updateButton->setText(tr("Update"));
        updateButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        updateButton->setStyleSheet(PACKAGE_UPDATE_STYLESHEET);
        connect(updateButton, SIGNAL(released()), this, SLOT(updateButtonHandler()));

        removeButton = new QPushButton(this);
        removeButton->setText(tr("Remove"));
        removeButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        removeButton->setStyleSheet(PACKAGE_REMOVE_STYLESHEET);
        connect(removeButton, SIGNAL(released()), this, SLOT(removeButtonHandler()));

        installButton = new QPushButton(this);
        installButton->setText(tr("Install"));
        installButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        installButton->setStyleSheet(PACKAGE_INSTALL_STYLESHEET);
        connect(installButton, SIGNAL(released()), this, SLOT(installButtonHandler()));

        upToDateButton = new QPushButton(this);
        upToDateButton->setText(tr("Up to date"));
        upToDateButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        upToDateButton->setStyleSheet(PACKAGE_UPTODATE_STYLESHEET);

        reloadButtonsStatus();
    }

    QMovie *movie;
    QLabel *processLabel;
    bool removeButtonEnable = false;
    bool allButtonEnable = false;
    QMetaObject::Connection failedProgressConnection;
    QPushButton *updateButton, *removeButton, *installButton, *upToDateButton, *cancelButton;
    QLabel *iconButton;
    QHBoxLayout *iconLayout;
    QString debugMessage;
    FileDownloader *m_pImgCtrl;
    Package *package;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
    TerminalWidget *terminal = nullptr;
};


#endif //PX_SOFTWARE_PACKAGECOMPONENT_H
