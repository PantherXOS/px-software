//
// Created by Reza Alizadeh Majd on 12/1/19.
//

#include "GuixPackageRemoveTask.h"

namespace PKG {
    GuixPackageRemovalTask::GuixPackageRemovalTask(const QString &packageName, QObject *parent) :
            GuixTask(QStringList() << "package" << "--remove" << packageName, parent),
            m_packageName(packageName) {}

    void GuixPackageRemovalTask::parseWorkerOutput(const QString &outData, const QString &errData) {
        emit packageRemoved(m_packageName);
    }
}
