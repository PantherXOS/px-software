//
// Created by Reza Alizadeh Majd on 11/10/19.
// Modified by Hamzeh Nasajpour on 04/05/20
//

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
