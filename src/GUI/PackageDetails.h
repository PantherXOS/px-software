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
#include "ScreenshotItem.h"
#include "PackageComponent.h"
#include "Settings.h"

using namespace PKG;

class PackageDetails : public PxQScrollArea{
    Q_OBJECT
public:
    PackageDetails(Package *package, const QString &title, PxQScrollArea *parent = nullptr);

private slots:
    void screenshotsDownloaded(const QString &);
    void onScreenshotClicked(QListWidgetItem*);

private:
    QVBoxLayout *loadRightSide();
    ScreenshotItem * downloadScreenshots(const QUrl &url);

    Package *package;
    FileDownloader * screenshotDownloader;
    map<QString , ScreenshotItem *> screenshotMap;
};


#endif //PX_SOFTWARE_PACKAGEDETAILS_H
