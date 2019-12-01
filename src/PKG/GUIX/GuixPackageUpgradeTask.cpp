//
// Created by Reza Alizadeh Majd on 12/1/19.
//

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
