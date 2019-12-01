//
// Created by Reza Alizadeh Majd on 12/1/19.
//

#include "GuixPackageInstallTask.h"

namespace PKG {
    GuixPackageInstallTask::GuixPackageInstallTask(const QString &packageName, QObject *parent) :
            GuixTask(QStringList() << "package" << "--install" << packageName, parent),
            m_packageName(packageName) {}

    void GuixPackageInstallTask::parseWorkerOutput(const QString &outData, const QString &errData) {
        emit packageInstalled(m_packageName);
    }
}
