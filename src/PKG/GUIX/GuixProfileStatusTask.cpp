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

#include "GuixProfileStatusTask.h"
#include "GuixInstalledPackagesTask.h"
#include "GuixUpgradablePackagesTask.h"

namespace PKG {
    GuixProfileStatusTask::GuixProfileStatusTask(QObject *parent) :
            GuixTask(QStringList(), parent),
            m_installedWorker(new GuixInstalledPackagesTask(this)),
            m_upgradableWorker(new GuixUpgradablePackagesTask(GuixPackageProfiles::USER, this)) {

        connect(m_installedWorker, &GuixInstalledPackagesTask::packageListReady,
                this, &GuixProfileStatusTask::onInstalledPackagesReady);
        connect(m_installedWorker, &AsyncTaskRunner::newData, this, &AsyncTaskRunner::newData);
        connect(m_installedWorker, &AsyncTaskRunner::failed, this, &AsyncTaskRunner::failed);

        connect(m_upgradableWorker, &GuixUpgradablePackagesTask::packageListReady,
                this, &GuixProfileStatusTask::onUpgradablePackagesReady);
        connect(m_upgradableWorker, &AsyncTaskRunner::newData, this, &AsyncTaskRunner::newData);
        connect(m_upgradableWorker, &AsyncTaskRunner::failed, this, &AsyncTaskRunner::failed);
    }

    bool GuixProfileStatusTask::asyncRun() {
        m_profile.clear();
        return m_installedWorker->asyncRun();
    }

    void GuixProfileStatusTask::onInstalledPackagesReady(const QStringList &installedPackages) {
        m_profile.installedPackages = installedPackages;
        m_upgradableWorker->asyncRun();
    }

    void GuixProfileStatusTask::onUpgradablePackagesReady(const QStringList &upgradablePackages) {
        m_profile.upgradablePackages = upgradablePackages;
        m_profile.refreshed = true;
        emit done("", "");
        emit profileIsReady(m_profile);
    }
}
