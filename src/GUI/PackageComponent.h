//
// Created by hamzeh on 12/6/19.
//

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

#include "FileDownloader.h"
#include "PackageManagerTracker.h"
#include "PackageManager.h"
#include "PxLineSeperator.h"
#include "TerminalWidget.h"
#include "Common.h"

class PackageComponent : public QWidget{
    Q_OBJECT
public:
    PackageComponent(Package *package, bool removeEnable, QWidget *parent = nullptr) : QWidget(parent){
        this->package = package;
        m_pkgMgrTrk = PackageManagerTracker::Instance();
        connect(m_pkgMgrTrk, SIGNAL(taskDataReceived(const QString&,const QString&)),this, SLOT(taskDataReceivedHandler(const QString,const QString&)));
        connect(m_pkgMgrTrk, SIGNAL(packageUpdated(const QString)),this, SLOT(packageUpdatedHandler(const QString)));
        connect(m_pkgMgrTrk, SIGNAL(packageRemoved(const QString)),this, SLOT(packageRemovedHandler(const QString)));
        connect(m_pkgMgrTrk, SIGNAL(packageInstalled(const QString)),this, SLOT(packageInstalledHandler(const QString)));
        connect(m_pkgMgrTrk, SIGNAL(packageTaskCanceled(const QString)),this, SLOT(taskCanceledHandler(const QString)));
        failedProgressConnection  = connect(m_pkgMgrTrk, SIGNAL(progressFailed(const QString&,const QString&)),this, SLOT(taskFailedHandler(const QString,const QString&)));

        this->removeButtonEnable = removeEnable;
        createButtonsLayout();
        createIconLayout(package->icon());
        this->terminal = new TerminalWidget(package->name());
    }

    QHBoxLayout *getIconLayout(){
        return iconLayout;
    }

    QHBoxLayout *getButtonsLayoutAsList(){
        auto buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(updateButton);
        buttonLayout->addWidget(removeButton);
        buttonLayout->addWidget(installButton);
        buttonLayout->addWidget(upToDateButton);
        buttonLayout->setAlignment(Qt::AlignRight | Qt::AlignCenter);
        return buttonLayout;
    }

    QVBoxLayout *getButtonsLayoutAsDetails(){
        auto buttonLayout = new QVBoxLayout;
        auto line = new PxLineSeperator(this);
        QLabel *version = new QLabel("Version : " + package->version(),this);
        QLabel *license = new QLabel("License : " + package->license(),this);

        buttonLayout->addWidget(updateButton);
        buttonLayout->addWidget(removeButton);
        buttonLayout->addWidget(installButton);
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

    void installButtonHandler(){
        if(m_pkgMgrTrk->requestPackageInstallation(package->name())) {
            reloadButtonsStatus();
        } else {
            m_pkgMgrTrk->requestPackageTaskCancel(package->name());
        }
    }

    void removeButtonHandler(){
        if(m_pkgMgrTrk->requestPackageRemoval(package->name())) {
            reloadButtonsStatus();
        } else {
            m_pkgMgrTrk->requestPackageTaskCancel(package->name());
        }
    }

    void updateButtonHandler(){
        if(m_pkgMgrTrk->requestPackageUpdate(package->name())) {
            reloadButtonsStatus();
        } else {
            m_pkgMgrTrk->requestPackageTaskCancel(package->name());
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
            reloadButtonsStatus();
        }
    }

    void packageRemovedHandler(const QString &name){
        if(name == package->name()){
            this->package->setInstalled(false);
            reloadButtonsStatus();
        }
    }

    void packageInstalledHandler(const QString &name){
        if(name == package->name()){
            this->package->setInstalled(true);
            reloadButtonsStatus();
        }
    }

    void taskCanceledHandler(const QString &name) {
        if(name == package->name()) {
            reloadButtonsStatus();
        }
    }

private:
    void createIconLayout(const QUrl &iconUrl){
        iconButton = new QLabel(this);
        iconButton->setFixedSize(QSize(PACKAGE_ICON_SIZE, PACKAGE_ICON_SIZE));
        iconButton->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        iconButton->setStyleSheet("QLabel {border 1px solid rgb(80, 80, 80);}");

        iconLayout = new QHBoxLayout;
        iconLayout->addWidget(iconButton);

        const char *homedir = getpwuid(getuid())->pw_dir;
        QString iconFileLocalPath = QString(homedir) + QString(PACKAGE_ICON_CACHE_DIR) + QString(this->package->name()) + QString("/");
        QString iconFilePath = iconFileLocalPath+iconUrl.fileName();
        QFile iconFile(iconFilePath);
        if(!iconFile.exists()){
            m_pImgCtrl = new FileDownloader(iconUrl,
                                            iconFileLocalPath,
                                            this);
            connect(m_pImgCtrl, SIGNAL (downloaded(const QString &)), this, SLOT (imageDownloaded(const QString &)));
        }
        imageDownloaded(iconFilePath);
        reloadButtonsStatus();
    }

    void createButtonsLayout(){
        // add install,update and remove buttons
        updateButton = new QPushButton(this);
        updateButton->setText("Update");
        updateButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        updateButton->setStyleSheet("QPushButton {background-color: green; color: white;}");
        connect(updateButton, SIGNAL(released()), this, SLOT(updateButtonHandler()));

        removeButton = new QPushButton(this);
        removeButton->setText("Remove");
        removeButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        removeButton->setStyleSheet("QPushButton {background-color: red; color: white;}");
        connect(removeButton, SIGNAL(released()), this, SLOT(removeButtonHandler()));

        installButton = new QPushButton(this);
        installButton->setText("Install");
        installButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        installButton->setStyleSheet("QPushButton {background-color: blue; color: white;}");
        connect(installButton, SIGNAL(released()), this, SLOT(installButtonHandler()));

        upToDateButton = new QPushButton(this);
        upToDateButton->setText("Up-To-Date");
        upToDateButton->setFixedSize(PACKAGE_BUTTON_W,PACKAGE_BUTTON_H);
        upToDateButton->setStyleSheet("QPushButton {background-color: gray; color: black;}");

        reloadButtonsStatus();
    }

    void reloadButtonsStatus(){
        updateButton->setVisible(false);
        removeButton->setVisible(false);
        upToDateButton->setVisible(false);
        installButton->setVisible(false);
        QIcon stopIcon(":images/general/src/GUI/resources/stop");
        QSize stopIconSize(20,20);
        if(m_pkgMgrTrk->inInstalling(package->name())) {
            installButton->setText("Installing ...");
            installButton->setIcon(stopIcon);
            installButton->setIconSize(stopIconSize);
            installButton->setVisible(true);
        } else if(m_pkgMgrTrk->inRemoving(package->name())) {
            removeButton->setText("Removing ...");
            removeButton->setIcon(stopIcon);
            removeButton->setIconSize(stopIconSize);
            removeButton->setVisible(true);
        } else if(m_pkgMgrTrk->inUpdating(package->name())) {
            updateButton->setText("Updating ...");
            updateButton->setIcon(stopIcon);
            updateButton->setIconSize(stopIconSize);
            updateButton->setVisible(true);
        } else {
            if(package->isInstalled()) {
                if (package->isUpdateAvailable()) {
                    updateButton->setText("Update");
                    updateButton->setIcon(QIcon());
                    updateButton->setVisible(true);
                }
                if(removeButtonEnable){
                    removeButton->setText("Remove");
                    removeButton->setIcon(QIcon());
                    removeButton->setVisible(true);
                }
                if(!removeButtonEnable && !(package->isUpdateAvailable())) {
                    upToDateButton->setText("Up-To-Date");
                    upToDateButton->setIcon(QIcon());
                    upToDateButton->setVisible(true);
                }
            } else {
                installButton->setText("Install");
                installButton->setIcon(QIcon());
                installButton->setVisible(true);
            }
        }
    }

    bool removeButtonEnable;
    QMetaObject::Connection failedProgressConnection;
    QPushButton *updateButton, *removeButton, *installButton, *upToDateButton;
    QLabel *iconButton;
    QHBoxLayout *iconLayout;
    QString debugMessage;
    FileDownloader *m_pImgCtrl;
    Package *package;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
    TerminalWidget *terminal = nullptr;
};


#endif //PX_SOFTWARE_PACKAGECOMPONENT_H
