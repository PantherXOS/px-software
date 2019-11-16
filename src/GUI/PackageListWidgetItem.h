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
#include "TerminalWidget.h"

using namespace std;
using namespace PKG;
class PackageListWidgetItem :public QWidget {
    Q_OBJECT
public:
    PackageListWidgetItem(Package *package, bool removeEnable, QWidget *parent = nullptr);

private slots:
    void imageDownloaded();
    void installButtonHandler();
    void removeButtonHandler();
    void updateButtonHandler();
    void taskFailedHandler(const QString &name, const QString &message);
    void packageProgressDoneHandler(const QString &name);
    void packageDetailReadyHandler(const QUuid &, Package *);
    void taskDataReceivedHandler(const QString,const QString&);
private:
    QMetaObject::Connection packageProgressConnection;
    QMetaObject::Connection failedProgressConnection;
    QMetaObject::Connection packageReadyConnection;
    QHBoxLayout *loadIcon();
    QVBoxLayout *loadTexts();
    QHBoxLayout *loadButtons();
    void reloadButtonsStatus();
    void reloadPackage();
    QPushButton *updateButton, *removeButton, *installButton, *upToDateButton;
    Package *package;
    QLabel *iconButton;
    QUrl iconRemoteUrl;
    FileDownloader *m_pImgCtrl;
    bool removeButtonEnable;
    PackageManager *m_pkgMgr = nullptr;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
    TerminalWidget *terminal;
};


#endif //PX_SOFTWARE_PACKAGELISTWIDGETITEM_H
