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

    void reloadButtonsStatus(){
        updateButton->setVisible(false);
        removeButton->setVisible(false);
        upToDateButton->setVisible(false);
        installButton->setVisible(false);
        QIcon stopIcon(":images/general/src/GUI/resources/stop");
        QSize stopIconSize(PACKAGE_STOP_PROGRESS_ICON_SIZE,PACKAGE_STOP_PROGRESS_ICON_SIZE);
        if(m_pkgMgrTrk->inInstalling(package->name())) {
            installButton->setText(tr("Installing ..."));
            installButton->setIcon(stopIcon);
            installButton->setIconSize(stopIconSize);
            installButton->setStyleSheet(PACKAGE_INPROGRESS_STYLESHEET);
            installButton->setVisible(true);
        } else if(m_pkgMgrTrk->inRemoving(package->name())) {
            removeButton->setText(tr("Removing ..."));
            removeButton->setIcon(stopIcon);
            removeButton->setIconSize(stopIconSize);
            removeButton->setStyleSheet(PACKAGE_INPROGRESS_STYLESHEET);
            removeButton->setVisible(true);
        } else if(m_pkgMgrTrk->inUpdating(package->name())) {
            updateButton->setText(tr("Updating ..."));
            updateButton->setIcon(stopIcon);
            updateButton->setIconSize(stopIconSize);
            updateButton->setStyleSheet(PACKAGE_INPROGRESS_STYLESHEET);
            updateButton->setVisible(true);
        } else {
            if(allButtonEnable){
                if(package->isInstalled()){
                    if(package->isUpdateAvailable()){
                        updateButton->setText(tr("Update"));
                        updateButton->setIcon(QIcon());
                        updateButton->setStyleSheet(PACKAGE_UPDATE_STYLESHEET);
                        updateButton->setVisible(true);
                    }
                    removeButton->setText(tr("Remove"));
                    removeButton->setIcon(QIcon());
                    removeButton->setStyleSheet(PACKAGE_REMOVE_STYLESHEET);
                    removeButton->setVisible(true);
                } else {
                    installButton->setText(tr("Install"));
                    installButton->setStyleSheet(PACKAGE_INSTALL_STYLESHEET);
                    installButton->setIcon(QIcon());
                    installButton->setVisible(true);
                }
            } else {
                if(package->isInstalled()) {
                    if (package->isUpdateAvailable()) {
                        updateButton->setText(tr("Update"));
                        updateButton->setIcon(QIcon());
                        updateButton->setStyleSheet(PACKAGE_UPDATE_STYLESHEET);
                        updateButton->setVisible(true);
                    } else if(removeButtonEnable) {
                        removeButton->setText(tr("Remove"));
                        removeButton->setIcon(QIcon());
                        removeButton->setStyleSheet(PACKAGE_REMOVE_STYLESHEET);
                        removeButton->setVisible(true);
                    } else {
                        upToDateButton->setText(tr("Up to date"));
                        upToDateButton->setIcon(QIcon());
                        upToDateButton->setStyleSheet(PACKAGE_UPTODATE_STYLESHEET);
                        upToDateButton->setVisible(true);
                    }
                } else {
                    installButton->setText(tr("Install"));
                    installButton->setStyleSheet(PACKAGE_INSTALL_STYLESHEET);
                    installButton->setIcon(QIcon());
                    installButton->setVisible(true);
                }
            }
        }
    }

    bool removeButtonEnable = false;
    bool allButtonEnable = false;
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
