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

#include "FileDownloader.h"
#include "PxQScrollArea.h"
#include "PackageManagerTracker.h"
#include "PackageManager.h"
#include "PxLineSeperator.h"

using namespace PKG;

class PackageDetails : public PxQScrollArea{
    Q_OBJECT
public:
    PackageDetails(Package *package, int id, QString title, PxQScrollArea * parent= nullptr);

private slots:
    void imageDownloaded();
    void installButtonHandler();
    void removeButtonHandler();
    void updateButtonHandler();
    void taskFailedHandler(const QString &name, const QString &message);
    void packageUpdatedHandler(const QString &name);
    void packageRemovedHandler(const QString &name);
    void packageInstalledHandler(const QString &name);

private:
    QHBoxLayout *loadIcon(const QUrl &iconUrl);
    QVBoxLayout *loadTexts();
    QVBoxLayout * loadButtons();
    void reloadButtonsStatus();

    QMetaObject::Connection failedProgressConnection;
    QPushButton *updateButton, *removeButton, *installButton, *upToDateButton;
    Package *package;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
    FileDownloader *m_pImgCtrl;
    QLabel *iconButton;
};


#endif //PX_SOFTWARE_PACKAGEDETAILS_H
