//
// Created by hamzeh on 11/23/19.
//

#ifndef PX_SOFTWARE_PACKAGEDETAILS_H
#define PX_SOFTWARE_PACKAGEDETAILS_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <pwd.h>
#include <zconf.h>
#include <QListView>
#include <QStandardItemModel>
#include <QListWidget>
#include <QListWidgetItem>

#include "FileDownloader.h"
#include "PxQScrollArea.h"
#include "PackageManagerTracker.h"
#include "PackageManager.h"
#include "PxLineSeperator.h"

using namespace PKG;

class PackageDetails : public PxQScrollArea{
    Q_OBJECT
public:
    PackageDetails(Package *package, const int id, const QString &title, PxQScrollArea *parent = nullptr);

private slots:
    void imageDownloaded(const QString & localfile);
    void screenshotsDownloaded(const QString &);
    void installButtonHandler();
    void removeButtonHandler();
    void updateButtonHandler();
    void taskFailedHandler(const QString &name, const QString &message);
    void packageUpdatedHandler(const QString &name);
    void packageRemovedHandler(const QString &name);
    void packageInstalledHandler(const QString &name);

private:
    QHBoxLayout *loadIcon(const QUrl &iconUrl);
    QVBoxLayout *loadRightSide();
    QVBoxLayout *loadButtons();
    void reloadButtonsStatus();
    void downloadScreenshots(const QUrl &url);

    QMetaObject::Connection failedProgressConnection;
    QPushButton *updateButton, *removeButton, *installButton, *upToDateButton;
    Package *package;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
    FileDownloader *m_pImgCtrl;
    FileDownloader * screenshotDownloader;
    map<QString , QListWidgetItem *> screenshotMap;
    QLabel *iconButton;
};


#endif //PX_SOFTWARE_PACKAGEDETAILS_H
