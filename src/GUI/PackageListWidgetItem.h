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
    Package * & getPackage();
    TerminalWidget * getTerminal();

private slots:
    void imageDownloaded(QString localfile);
    void installButtonHandler();
    void removeButtonHandler();
    void updateButtonHandler();
    void taskFailedHandler(const QString &name, const QString &message);
    void packageUpdatedHandler(const QString &name);
    void packageRemovedHandler(const QString &name);
    void packageInstalledHandler(const QString &name);
    void taskDataReceivedHandler(const QString &, const QString &);

private:
    QHBoxLayout *loadIcon(const QUrl &iconUrl);
    QVBoxLayout *loadTexts();
    QHBoxLayout *loadButtons();
    void reloadButtonsStatus();

    QMetaObject::Connection failedProgressConnection;
    QPushButton *updateButton, *removeButton, *installButton, *upToDateButton;
    bool removeButtonEnable;
    Package *package;
    QLabel *iconButton;
    FileDownloader *m_pImgCtrl;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
    TerminalWidget *terminal = nullptr;
    QString debugMessage;
};


#endif //PX_SOFTWARE_PACKAGELISTWIDGETITEM_H
