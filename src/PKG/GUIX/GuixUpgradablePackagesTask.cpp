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

#include "GuixUpgradablePackagesTask.h"

namespace PKG {
    GuixUpgradablePackagesTask::GuixUpgradablePackagesTask(GuixPackageProfiles profile, QObject *parent) :
            PxTask(((profile==GuixPackageProfiles::SYSTEM)?QStringList()<<"system":QStringList()), parent),
            m_profile(profile) {
    }

    void GuixUpgradablePackagesTask::parseWorkerOutput(const QString &outData, const QString &errData) {
        QStringList guixPackages;
        for (const auto &line : outData.split('\n')) {
                auto parts = line.trimmed().split(':');
                if (parts.size() == 2) {
                    guixPackages << parts[0];
                }
        }
        emit packageListReady(guixPackages);
    }
}
