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

#include "GuixPackageUpgradeTask.h"

namespace PKG {
    GuixPackageUpdateTask::GuixPackageUpdateTask(const QStringList &packageList, QObject *parent) :
            GuixTask(QStringList(), parent),
            m_packageList(packageList) {
        m_appArgs << "package" << "--upgrade";
        for (const auto &pkg : packageList) {
            m_appArgs << pkg;
        }
    }

    void GuixPackageUpdateTask::parseWorkerOutput(const QString &outData, const QString &errData) {
        emit packagesUpdated(m_packageList);
    }
}
