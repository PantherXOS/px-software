//
// Created by hamzeh on 11/3/19.
//

#ifndef PX_SOFTWARE_PACKAGEWIDGET_H
#define PX_SOFTWARE_PACKAGEWIDGET_H
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

using namespace std;
class PackageWidget :public QWidget {
    Q_OBJECT
public:
    PackageWidget(PKG::Package *package, bool removeEnable, QWidget *parent = nullptr);

private slots:
    void imageDownloaded();
    void installButtonHandler();
    void removeButtonHandler();
    void updateButtonHandler();
    void taskFailedHandler(const QUuid &, const QString &message);
    void taskDoneHandler(const QUuid &, const QString &message);
    void packagedInstalledHandler(const QString &name);
    void packagedRemovedHandler(const QString &name);
    void packagedUpdatedHandler(const QStringList &nameList);
    void taskDataHandler(const QUuid &taskId, const QString &data);

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
};


#endif //PX_SOFTWARE_PACKAGEWIDGET_H
