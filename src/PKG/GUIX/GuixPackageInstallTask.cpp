//
// Created by Reza Alizadeh Majd on 12/1/19.
//

#include "GuixPackageInstallTask.h"
#include "src/PKG/MISC/LxqtSessionInterface.h"

namespace PKG {
    GuixPackageInstallTask::GuixPackageInstallTask(const QString &packageName, QObject *parent) :
            GuixTask(QStringList() << "package" << "--install" << packageName, parent),
            m_packageName(packageName) {
        connect(this, &AsyncTaskRunner::done, [=]() {
            auto sessionInterface = new LxqtSessionInterface(this);
            connect(sessionInterface, &LxqtSessionInterface::panelRestarted, sessionInterface, &QObject::deleteLater);
            sessionInterface->restartPanel();
        });
    }

    void GuixPackageInstallTask::parseWorkerOutput(const QString &outData, const QString &errData) {
        emit packageInstalled(m_packageName);
    }
}
