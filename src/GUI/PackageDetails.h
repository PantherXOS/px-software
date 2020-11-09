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
#include "CacheManager.h"

using namespace PKG;

class PackageDetails : public PxQScrollArea{
    Q_OBJECT
public:
    PackageDetails(Package *package, const QString &title, PxQScrollArea *parent = nullptr);

private slots:
    void screenshotsDownloaded(const QString &);
    void onScreenshotClicked(QListWidgetItem*);
    void showTerminalSignalHandler(TerminalWidget *);

signals:
    void screenshotItemClicked(ScreenshotItem *sci);
    void showTerminalSignal(TerminalWidget *);

private:
    QVBoxLayout *loadRightSide();
    QListWidget *createScreenshotList(const QStringList &list);

    QColor bgColor,fgColor;
    Package *package;
    FileDownloader * screenshotDownloader;
    map<QString , ScreenshotItem *> screenshotMap;
};


#endif //PX_SOFTWARE_PACKAGEDETAILS_H
