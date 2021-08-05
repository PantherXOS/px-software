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

#include <QDebug>
#include "GuixPackageSearchTask.h"
#include "MISC/Utils.h"

#define MAX_SEARCH_LIMIT                20
#define TEMP_SEARCH_RESULT_FILE_NAME    QString("/tmp/px-software-package-search-file")

namespace PKG {
    GuixPackageSearchTask::GuixPackageSearchTask(const QString &packageName, QObject *parent) :
            GuixTask(QStringList() << "package" << "-s" << packageName, parent),
            m_packageName(packageName),
            m_parent(parent) {
    }

    void GuixPackageSearchTask::parseWorkerOutput(const QString &outData, const QString &errData) {
        RecDB db;
        QVector <Package *> packageList;
        // -------------------------------------------------------------- Store the result to temp file to retrieve and limit
        QFile file(TEMP_SEARCH_RESULT_FILE_NAME);
        file.open(QIODevice::WriteOnly);
        file.write(outData.toLatin1());
        file.close();
        //
        bool result = false;
        QString searchCommand = "recsel -n 0-" + QString::number(MAX_SEARCH_LIMIT-1) + " " + TEMP_SEARCH_RESULT_FILE_NAME;
        QString searchResult = QString(PXUTILS::COMMAND::Execute(searchCommand.toStdString().c_str(), result).c_str());
        // -----------------------------------------------------------------------------------------------------------------
        auto recs = db.findFromText(searchResult,"");
        int i = 0;
        for (auto const &rec : recs) {
            auto *pkg = Package::MakePackage(rec,m_parent);
            if(pkg!=nullptr) {
                i++;
                if(pkg->title().isEmpty())
                    pkg->setTitle(pkg->name());
                packageList.append(pkg);
            }
            if(i >= MAX_SEARCH_LIMIT) {
                qWarning() << "LIMIT: Search results exceeded to" << MAX_SEARCH_LIMIT;
                break;
            }
        }
        emit searchResultReady(packageList);
    }
}
