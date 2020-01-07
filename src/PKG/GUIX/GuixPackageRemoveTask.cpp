//
// Created by Reza Alizadeh Majd on 12/1/19.
//

#include "GuixPackageRemoveTask.h"
#include "src/PKG/MISC/LxqtSessionInterface.h"

namespace PKG {
    GuixPackageRemovalTask::GuixPackageRemovalTask(const QString &packageName, QObject *parent) :
            GuixTask(QStringList() << "package" << "--remove" << packageName, parent),
            m_packageName(packageName) {
        connect(this, &AsyncTaskRunner::done, [=]() {
            auto sessionInterface = new LxqtSessionInterface(this);
            connect(sessionInterface, &LxqtSessionInterface::panelRestarted, sessionInterface, &QObject::deleteLater);
            sessionInterface->restartPanel();
        });
    }

    void GuixPackageRemovalTask::parseWorkerOutput(const QString &outData, const QString &errData) {
        emit packageRemoved(m_packageName);
    }
}
