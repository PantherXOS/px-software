//
// Created by Reza Alizadeh Majd on 11/10/19.
//

#include "GuixUpgradablePackagesTask.h"

namespace PKG {
    GuixUpgradablePackagesTask::GuixUpgradablePackagesTask(GuixPackageProfiles profile, QObject *parent) :
            GuixTask(QStringList(), parent),
            m_profile(profile) {
        m_appArgs << "package";
        if (m_profile == GuixPackageProfiles::SYSTEM) {
            m_appArgs << "-p" << "/run/current-system/profile";
        }
        m_appArgs << "-n" << "-u";
    }

    void GuixUpgradablePackagesTask::parseWorkerOutput(const QString &outData, const QString &errData) {
        QStringList guixPackages;
        for (const auto &line : errData.split('\n')) {
            if (line.startsWith("   ")) {
                auto parts = line.trimmed().split('\t');
                if (parts.size() == 3) {
                    guixPackages << parts[0];
                }
            }
        }
        emit packageListReady(guixPackages);
    }
}
