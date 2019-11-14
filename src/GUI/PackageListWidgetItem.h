//
// Created by hamzeh on 11/3/19.
//

#ifndef PX_SOFTWARE_PACKAGELISTWIDGETITEM_H
#define PX_SOFTWARE_PACKAGELISTWIDGETITEM_H
#include <iostream>
#include <string>
#include <QWidget>
#include <QString>
#include <vector>
#include <QLabel>
#include <QImage>
#include <QIcon>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <QLineEdit>

#include "DataEntities.h"
#include "FileDownloader.h"
#include "PackageManager.h"
#include "PackageManagerTracker.h"

using namespace std;
class PackageListWidgetItem :public QWidget {
    Q_OBJECT
public:
    PackageListWidgetItem(PKG::Package *package, bool removeEnable, QWidget *parent = nullptr);

private slots:
    void imageDownloaded();
    void installButtonHandler();
    void removeButtonHandler();
    void updateButtonHandler();
    void taskFailedHandler(const QString &name, const QString &message);
    void packagedInstalledHandler(const QString &name);
    void packagedRemovedHandler(const QString &name);
    void packagedUpdatedHandler(const QString &name);

private:
    QMetaObject::Connection updateSignalConnection;
    QMetaObject::Connection installationSignalConnection;
    QMetaObject::Connection removeSignalConnection;
    QMetaObject::Connection failedTaskSignalConnection;
    QMetaObject::Connection dataReceivedConnection;
    QMetaObject::Connection taskDoneSignalConnection;
    QHBoxLayout *loadIcon();
    QVBoxLayout *loadTexts();
    QHBoxLayout *loadButtons();
    void reloadButtonsStatus();
    void reloadPackage();
    QPushButton *updateButton, *removeButton, *installButton;
    PKG::Package *package;
    QLabel *iconButton;
    QUrl iconRemoteUrl;
    FileDownloader *m_pImgCtrl;
    bool removeButtonEnable;
    PKG::PackageManager *m_pkgMgr = nullptr;
//    PackageManagerTracker *m_pkgMgrTrk = nullptr;
};


#endif //PX_SOFTWARE_PACKAGELISTWIDGETITEM_H
