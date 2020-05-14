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

#ifndef PX_SOFTWARE_GUIXPROFILESTATUSTASK_H
#define PX_SOFTWARE_GUIXPROFILESTATUSTASK_H

#include "GuixTask.h"

namespace PKG {
    class GuixInstalledPackagesTask;
    class GuixUpgradablePackagesTask;

    struct GuixProfile {
        QStringList installedPackages;
        QStringList upgradablePackages;
        bool refreshed = false;

        void clear() {
            installedPackages.clear();
            upgradablePackages.clear();
            refreshed = false;
        }
    };

    class GuixProfileStatusTask: public GuixTask {
        Q_OBJECT
    public:
        explicit GuixProfileStatusTask(QObject *parent = nullptr);
        bool asyncRun() override;

    protected slots:
        void parseWorkerOutput(const QString &outData, const QString &errData) override {};
        void onInstalledPackagesReady(const QStringList &installedPackages);
        void onUpgradablePackagesReady(const QStringList &upgradablePackages);

    signals:
        void profileIsReady(const GuixProfile &profile);

    private:
        GuixInstalledPackagesTask *m_installedWorker;
        GuixUpgradablePackagesTask *m_upgradableWorker;
        GuixProfile m_profile;
    };
}

#endif //PX_SOFTWARE_GUIXPROFILESTATUSTASK_H
